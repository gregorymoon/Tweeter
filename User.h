#ifndef USER_H
#define USER_H

#include <string>
#include <queue>

#include "Tweet.h"

//Commands:
//0 = Start
//1 = Follow
//2 = Read

class User{
	public:
		User(std::string handleInput, std::queue<Tweet *> tweets, std::queue<std::string> tagsToFollow, std::queue<int> commands);
		~User();

		std::string getHandle();
		
		void start();
	private:
		std::string handle;
		
		std::queue<Tweet *> tweets;

		std::queue<int> commands;
		
		std::queue<std::string> tagsToFollow;
		void initializeBuffer();
};

#endif
