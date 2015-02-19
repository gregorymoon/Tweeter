#ifndef TWEET_H
#define TWEET_H

#include <string>
#include <vector>

class Tweet{
	public:
		Tweet(std::string hashtagInput, std::vector<std::string> contentsInput);
		~Tweet();
		
		std::string getHashtag();
		std::vector<std::string> getContents();
	private:
		std::string hashtag;
		std::vector<std::string> contents;
};

#endif
