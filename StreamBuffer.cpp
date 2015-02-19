#include "StreamBuffer.h"

StreamBuffer::StreamBuffer(){
}

StreamBuffer::~StreamBuffer(){
}

std::queue<Tweet> StreamBuffer::getFollowStream(){
	return followStream;
}

std::queue<Tweet> StreamBuffer::getTweetStream(){
	return tweetStream;
}
