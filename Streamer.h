#ifndef STREAMER_H
#define STREAMER_H

#include <string>

#include "StreamBuffer.h"

class Streamer{
	public:
		static void start();	
		static void addBuffer(std::string key);
		static void join();
	private:
		static void *runThreadFunctions(void *params);
};

#endif
