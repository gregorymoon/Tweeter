#include <pthread.h>
#include <fstream>
#include <semaphore.h>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <unordered_map>

struct Tweet{
	std::vector<std::string> lines;
	std::string owner;
};

struct StreamBuffer{
	std::vector<std::string> streamTweetBuff, followTweetBuff;
};

struct UserInfo{
	sem_t userStreamerSem, streamingLineSem;
	std::string handle, tagToFollow, currLine, currTag, currOwner;
	char  *filename;
	StreamBuffer *buff = new StreamBuffer();
	bool sendingTweet = false, followingTag = false, exiting = false;
};

struct StreamerInfo{
	sem_t userNumModSem, streamerTweeterSem, followingTweetsSem, sendingLineSem;
        std::vector<UserInfo *> users;
        bool gettingTweet = false, exiting = false, sendingTweet = false, receivingTweet = false;
        std::string tagToFollow, currTag, currLine, currOwner, currUser;
	int numUsers = 10, numTweets, numLines;
};

struct TweeterInfo{
	StreamerInfo *sInfo;
};

void *startUserThread(void *params);
void *startStreamerThread(void *params);
void *startTweeterThread(void *params);


bool splitString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ");
void removeUser(StreamerInfo *sInfo, UserInfo *userToRemove);
void decNumUsers(StreamerInfo *sInfo);
void resetSem(sem_t sem, int val);

std::vector<pthread_t> userThreads;
pthread_t tweeterThread, streamerThread;


int main(int argc, char* argv[]){
	char* inputFiles[] = {
		"user0.txt", "user1.txt", "user2.txt", "user3.txt",
		"user4.txt", "user5.txt", "user6.txt", "user7.txt",
		"user8.txt", "user9.txt", "user10.txt"
	};
	int numUsers = atoi(argv[1]);

	sem_t userNumModSem, streamerTweeterSem, followingTweetsSem, sendingLineSem;
	sem_init(&userNumModSem, 0, 1);
	sem_init(&streamerTweeterSem, 0, 0);
	sem_init(&followingTweetsSem, 0, 0);
	sem_init(&sendingLineSem, 0, 0);

	StreamerInfo *sInfo = new StreamerInfo();
	sInfo->numUsers = numUsers;
	sInfo->userNumModSem = userNumModSem;
	sInfo->streamerTweeterSem = streamerTweeterSem;
	sInfo->followingTweetsSem = followingTweetsSem;
	sInfo->sendingLineSem = sendingLineSem;

	TweeterInfo *tInfo = new TweeterInfo();
	tInfo->sInfo = sInfo;

	for(int i = 0; i < numUsers; i++)
		sInfo->users.push_back(new UserInfo());
	

	pthread_t streamerThread, tweeterThread;
	pthread_create(&streamerThread, NULL, startStreamerThread, (void *)sInfo);
	pthread_create(&tweeterThread, NULL, startTweeterThread, (void *)tInfo);

	for(int i = 0; i < numUsers; i++){
		char* filename = (char *)malloc(sizeof(inputFiles[i]));
		strcpy(filename, inputFiles[i]);
		sem_t userStreamerSem, streamingLineSem;
		sem_init(&userStreamerSem, 0, 0);
		sem_init(&streamingLineSem, 0, 0);

		UserInfo *userInfo = sInfo->users[i];
		userInfo->filename = filename;
		userInfo->userStreamerSem = userStreamerSem;
		userInfo->streamingLineSem = streamingLineSem;
	
		pthread_t userThread;
		pthread_create(&userThread, NULL, startUserThread, (void*)userInfo);
		userThreads.push_back(userThread);
	}

	for(int i = 0; i < userThreads.size(); i++)
		pthread_join(userThreads[i], NULL);
	
	pthread_join(streamerThread, NULL);
	pthread_join(tweeterThread, NULL);
	
	return 1;
}

void *startUserThread(void *params){
	UserInfo *userInfo = (UserInfo *)params;
	char* filename = userInfo->filename;
	char line[1024];
	std::filebuf fBuff;

	if(fBuff.open(filename, std::ios::in))
  	{
    		std::istream fileStream(&fBuff);
    		while(!fileStream.eof()){
      			fileStream.getline(line, sizeof(line));
			std::vector<std::string> tokens;
			std::string lineString(line);
			
			if(!splitString(lineString, tokens, " "))
				continue;

			std::string command = tokens[0];
	
			if(command == "Handle"){
				userInfo->handle = tokens[1];
				printf("creating user %s\n", userInfo->handle.c_str());
			}
			else if(command == "Start"){
				printf("%s sending tweet\n", userInfo->handle.c_str());

				userInfo->currTag = tokens[1];

				while(1){
					fileStream.getline(line, sizeof(line));
					lineString = std::string(line);
					std::vector<std::string> tempTokens;
					splitString(lineString, tempTokens, " ");
					
					if(tempTokens[0] == "End"){
						userInfo->sendingTweet = false;
						sem_post(&userInfo->streamingLineSem);
						break;
					}
		
					if(!userInfo->sendingTweet){
						userInfo->sendingTweet = true;
						sem_wait(&userInfo->userStreamerSem);
					}

					userInfo->currLine = lineString;
					printf("%s: %s\n", userInfo->handle.c_str(), line);
					sem_post(&userInfo->streamingLineSem);
					sem_wait(&userInfo->userStreamerSem);
				}

			}
			else if(command == "Follow"){
				printf("%s following tag\n", userInfo->handle.c_str());
				userInfo->tagToFollow = tokens[1];
				userInfo->followingTag = true;
				sem_wait(&userInfo->userStreamerSem);
				userInfo->followingTag = false;
				sem_wait(&userInfo->userStreamerSem);

			}
			else if(command == "Read"){
				printf("%s reading for 2 seconds\n", userInfo->handle.c_str());
				sleep(2);
			}
			else if(command == "Exit"){
				userInfo->exiting = true;
				sem_wait(&userInfo->userStreamerSem);
				printf("%s exiting gracefully\n", userInfo->handle.c_str());
				userInfo->exiting = false;
			}
		}
    		fBuff.close();
  	}
}

void *startStreamerThread(void *params){
	StreamerInfo *streamerInfo = (StreamerInfo *)params;
	std::vector<UserInfo *> users = streamerInfo->users;
	printf("Starting Streamer\n");

	while(1){
		if(streamerInfo->numUsers == 0){
			streamerInfo->exiting = true;
			sem_wait(&streamerInfo->streamerTweeterSem);
			printf("Streamer exiting gracefully\n");
			break;
		}

		for(int i = 0; i < users.size(); i++){
			UserInfo *info = users[i];

			if(info->sendingTweet){
				std::string prevLine;
				streamerInfo->currOwner = info->handle;
				streamerInfo->currTag = info->currTag;
	
				sem_post(&info->userStreamerSem);
				while(info->sendingTweet){
					sem_wait(&info->streamingLineSem);
					
					if(info->sendingTweet){
						if(prevLine != info->currLine){
							prevLine = info->currLine;
							info->buff->streamTweetBuff.push_back(info->currLine);
						}
						sem_post(&info->userStreamerSem);
					}
				}

				streamerInfo->sendingTweet = true;
				sem_wait(&streamerInfo->streamerTweeterSem);

				for(int j = 0; j < info->buff->streamTweetBuff.size(); j++){
                                        streamerInfo->currLine = info->buff->streamTweetBuff[j];
                                        sem_post(&streamerInfo->sendingLineSem);
                                        sem_wait(&streamerInfo->streamerTweeterSem);
                                }

				info->buff->streamTweetBuff.clear();
                                streamerInfo->sendingTweet = false;
                            	sem_post(&streamerInfo->sendingLineSem);
	
			}

			if(info->followingTag){
				streamerInfo->tagToFollow = info->tagToFollow;
				streamerInfo->currUser = info->handle;
				sem_post(&info->userStreamerSem);
			
				streamerInfo->gettingTweet = true;
				sem_wait(&streamerInfo->followingTweetsSem);
				std::string prevLine;

				while(streamerInfo->numTweets > 0){
					while(streamerInfo->numLines > 0){
						if(prevLine != streamerInfo->currLine){
							prevLine = streamerInfo->currLine;
							printf("Streamer getting line from Tweeter: %s\n", streamerInfo->currLine.c_str());
						}
					}
					sem_post(&streamerInfo->sendingLineSem);
				}
		
				streamerInfo->gettingTweet = false;
				sem_wait(&streamerInfo->followingTweetsSem);

				while(info->followingTag){}
			
				sem_post(&info->userStreamerSem);
				
			}

			if(info->exiting){
				printf("streamer removing %s\n", info->handle.c_str());
				removeUser(streamerInfo, info);	
				sem_post(&info->userStreamerSem);
				while(info->exiting){}
			}
		}
	}
}

void *startTweeterThread(void *params){
	TweeterInfo *tInfo = (TweeterInfo *)params;
	StreamerInfo *sInfo = tInfo->sInfo;
	std::unordered_map<std::string, std::vector<Tweet *>> tweetRepo;

	printf("Starting Tweeter\n");

	while(1){
		if(sInfo->exiting){
			printf("Tweeter exiting gracefully\n");
			sem_post(&sInfo->streamerTweeterSem);
			break;
		}

		if(sInfo->sendingTweet){
			Tweet *newTweet = new Tweet();
			newTweet->owner = sInfo->currOwner;
			tweetRepo[sInfo->currTag].push_back(newTweet);
			std::string prevLine;

			sem_post(&sInfo->streamerTweeterSem);

			while(sInfo->sendingTweet){
				sem_wait(&sInfo->sendingLineSem);
		
				if(sInfo->sendingTweet){
					if(prevLine != sInfo->currLine){
						prevLine = sInfo->currLine;
						newTweet->lines.push_back(sInfo->currLine);
					}

					sem_post(&sInfo->streamerTweeterSem);
				}
			}
		}

		if(sInfo->gettingTweet){
			std::vector<Tweet *> tweets = tweetRepo[sInfo->tagToFollow];
			sInfo->numTweets = tweets.size();
			printf("%s following %s\n", sInfo->currUser.c_str(), sInfo->tagToFollow.c_str());
			sem_post(&sInfo->followingTweetsSem);

			for(int i = 0; i < tweets.size(); i++){
				Tweet *currTweet = tweets[i];
				printf("Sending tweet from %s to %s\n", currTweet->owner.c_str(), sInfo->currUser.c_str());
				sInfo->numLines = currTweet->lines.size();

				for(int j = 0; j < currTweet->lines.size(); j++){
					sInfo->numLines--;	
					printf("%s\n", currTweet->lines[j].c_str());
				}

				sInfo->numTweets--;
			}

			
			while(sInfo->gettingTweet){}
			sem_post(&sInfo->followingTweetsSem);
		}
	}
}

void resetSem(sem_t sem, int val){
	int tempVal;
	sem_getvalue(&sem, &tempVal);

	while(tempVal != val){
		if(tempVal <  val)
			sem_post(&sem);
		
		if(tempVal > val)
			sem_wait(&sem);

		sem_getvalue(&sem, &tempVal);
	}
}

void removeUser(StreamerInfo *sInfo, UserInfo *userToRemove){
	int idx = -1;
	std::vector<UserInfo *> users = sInfo->users;

	for(int i = 0; i < users.size(); i++){
		if(users[i]->handle == userToRemove->handle){
			idx = i;
			break;
		}
	}
	
	decNumUsers(sInfo);
	users.erase(users.begin() + idx);
}

void decNumUsers(StreamerInfo *sInfo){
	sem_wait(&sInfo->userNumModSem);
	sInfo->numUsers--;
	sem_post(&sInfo->userNumModSem);
}

bool splitString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters){
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    	while (std::string::npos != pos || std::string::npos != lastPos)
    	{
       		tokens.push_back(str.substr(lastPos, pos - lastPos));
       		lastPos = str.find_first_not_of(delimiters, pos);
    		pos = str.find_first_of(delimiters, lastPos);
	}

	if(tokens.size() == 0)
		return false;
	else
		return true;
}
