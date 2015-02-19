#include <cstdio>
#include <string>
#include <dirent.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

#include "User.h"
#include "Streamer.h"
#include "Tweet.h"

std::vector<User *> readAllUsersFromFile(std::string dirPath);
User* readUserFromFile(std::string filepath);
std::vector<std::string> split(std::string inputString);

int main(int argc, char* argv[]){
	//get dir location from user input
	std::string dirPath;

	if(argc == 2)
		dirPath = argv[1];
	else
		dirPath = "/home/grmoon/school/cse430/projects/2/sample_input/";
	
	std::vector<User *> users = readAllUsersFromFile(dirPath);

	//Streamer::start();

	//User *u1 = new User("handle");

	//Streamer::join();
	
	return 0;
}

std::vector<User *> readAllUsersFromFile(std::string dirPath){
	DIR *dir = opendir(dirPath.c_str());
	std::vector<User *> users;

	if(!dir){
		perror("opendir");
		exit(1);
	}
	else{
		struct dirent *dirEntry;
		while((dirEntry = readdir(dir)) != NULL){
			if(strstr(dirEntry->d_name, ".txt") != NULL){
				std::string filepath = dirPath + dirEntry->d_name;
				printf("Reading User from %s\n", dirEntry->d_name);
				users.push_back(readUserFromFile(filepath));
			}
		}
	
		closedir(dir);
	}		

	return users;
}

User* readUserFromFile(std::string filepath){
	std::ifstream currFile;
	currFile.open(filepath);
	std::string userHandle;
	User *userToAdd;

	if(currFile.is_open()){
		std::string currLine;
		std::queue<int> commands;
		std::queue<std::string> tagsToFollow;
		std::queue<Tweet *> tweets;

		while(std::getline(currFile, currLine)){
			std::vector<std::string> words = split(currLine);

			if(words[0].compare("Handle") == 0)
				userHandle = words[1];
			else if(words[0].compare("Start") == 0){
				commands.push(0);
				std::string hashtag = words[1];
				std::vector<std::string> linesInTweet;
				std::getline(currFile, currLine);
				words = split(currLine);

				while(words[0].compare("End") != 0){
					linesInTweet.push_back(currLine);
					std::getline(currFile, currLine);
					words = split(currLine);
				}
				Tweet *tweetToAdd = new Tweet(hashtag, linesInTweet);
				tweets.push(tweetToAdd);
			}			
			else if(words[0].compare("Follow") == 0){
				commands.push(1);
				tagsToFollow.push(words[1]);						
			}	
			else if(words[0].compare("Read") == 0)
				commands.push(2);
		}
		
		userToAdd = new User(userHandle, tweets, tagsToFollow, commands);
	}
	else{
		perror("openfile");
		exit(1);
	}
	
	currFile.close();
	return userToAdd;
}

std::vector<std::string> split(std::string inputString){
	std::vector<std::string> splitString;
	std::string currWord;
	std::stringstream inputStringStream(inputString);

	while(inputStringStream.good()){
		inputStringStream >> currWord;
		splitString.push_back(currWord);
	}

	return splitString;
}
