#ifndef STREAMBUFFER_H
#define STREAMBUFFER_H

#include <queue>

#include "Tweet.h"

class StreamBuffer{
	public:
		StreamBuffer();
		~StreamBuffer();
		
		std::queue<Tweet> getFollowStream();
		std::queue<Tweet> getTweetStream();
	private:
		std::queue<Tweet> followStream, tweetStream;
};

#endif
