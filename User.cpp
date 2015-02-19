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

void User::start(){

}

void User::initializeBuffer(){
	printf("Initializing Buffer for %s\n", handle.c_str());
	Streamer::addBuffer(handle);
}
