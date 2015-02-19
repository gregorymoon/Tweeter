#include "Tweet.h"

Tweet::Tweet(std::string hashtagInput, std::vector<std::string> contentsInput){
	this->hashtag = hashtagInput;
	this->contents = contentsInput;
}

Tweet::~Tweet(){
}

std::vector<std::string> Tweet::getContents(){
	return this->contents;
}

std::string Tweet::getHashtag(){
	return this->hashtag;
}
