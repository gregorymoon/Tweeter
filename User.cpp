#include <pthread.h>
#include <cstdio>

#include "User.h"
#include "Streamer.h"

User::User(std::string handleInput){
	printf("Creating User %s\n", handleInput.c_str());
	this->handle = handleInput;
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
