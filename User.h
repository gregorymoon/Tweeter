#ifndef USER_H
#define USER_H

#include <string>

#include "Tweet.h"

class User{
	public:
		User(std::string handleInput);
		~User();

		std::string getHandle();
		
		void start();
	private:
		std::string handle;

		void initializeBuffer();
};

#endif
