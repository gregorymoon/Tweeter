#include <pthread.h>
#include <cstdio>

#include "User.h"
#include "Streamer.h"

User::User(std::string handleInput, std::queue<Tweet *> tweets, std::queue<std::string> tagsToFollow, std::queue<int> commands){
	printf("Creating User %s\n", handleInput.c_str());
	this->handle = handleInput;
	this->commands = commands;
	this->tweets = tweets;
	this->tagsToFollow = tagsToFollow;

	this->initializeBuffer();
}

User::~User(){
}

std::string User::getHandle(){
	return handle;
}

void *User::start(void *userObject){
	return ((User *) userObject)->runCommands();
}

void *User::runCommands(){
	int command;

	while(!commands.empty()){
		command = commands.front();
		commands.pop();
		
		switch(command){
			case 0:
				send();				
				break;
			case 1:
				follow();
				break;
			case 2:
				read();
				break;
			default:
				printf("Something is wrong in User::start\n");
				exit(1);
		}
	}
}

void User::initializeBuffer(){
	printf("Initializing Buffer for %s\n", handle.c_str());
	Streamer::addBuffer(handle);
}

void User::follow(){
	std::string tagToFollow = tagsToFollow.front();
	tagsToFollow.pop();
	printf("%s following tag %s\n", handle.c_str(), tagToFollow.c_str());
}

void User::send(){
	Tweet *tweetToSend = tweets.front();
	tweets.pop();
	printf("%s sending Tweet %s\n", handle.c_str(), tweetToSend->getHashtag().c_str());
}

void User::read(){
	printf("%s reading for 2 secs\n", handle.c_str());
}
