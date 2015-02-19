#ifndef TWEETER_H
#define TWEETER_H

#include <unordered_map>
#include <string>

#include "Tweet.h"

class Tweeter{
	public:
		Tweeter();
		~Tweeter();
	private:
		std::unordered_map<std::string, Tweet *> tweetsByHashtag;
};

#endif
