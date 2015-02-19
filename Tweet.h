#ifndef TWEET_H
#define TWEET_H

#include <string>

class Tweet{
	public:
		Tweet(std::string hashtagInput, std::string contentsInput);
		~Tweet();
		
		std::string getHashtag();
		std::string getContents();
	private:
		std::string hashtag;
		std::string contents;
};

#endif
