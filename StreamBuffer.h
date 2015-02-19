#ifndef STREAMBUFFER_H
#define STREAMBUFFER_H

#include <queue>

#include "Tweet.h"

class StreamBuffer{
	public:
		StreamBuffer();
		~StreamBuffer();

		Tweet* getToStream();
		Tweet* getFromStream();
	private:
		Tweet *toStream, *fromStream;
};
#endif
