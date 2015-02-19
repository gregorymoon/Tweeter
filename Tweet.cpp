#include "Tweet.h"

Tweet::Tweet(std::string hashtagInput, std::string contentsInput){
	this->hashtag = hashtagInput;
	this->contents = contentsInput;
}

Tweet::~Tweet(){
}

std::string Tweet::getContents(){
	return this->contents;
}

std::string Tweet::getHashtag(){
	return this->hashtag;
}
