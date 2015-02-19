#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <unordered_map>
#include <unistd.h>

#include "Streamer.h"
#include "StreamBuffer.h"

std::unordered_map<std::string, StreamBuffer *> buffers;
pthread_t streamerThread;

void Streamer::start(){
	bool success = !pthread_create(&streamerThread, NULL, runThreadFunctions, NULL);
	
	if(success){
		printf("Starting Streamer\n");
		//pthread_join(streamerThread, NULL);
	}
	else{
		printf("Problem Starting Streamer\n");
		exit(EXIT_FAILURE);
	}
}

void *Streamer::runThreadFunctions(void *params){
	while(true){
		printf("Streamer Running\n");
		usleep(200000);
	}
}

void Streamer::addBuffer(std::string key){
	StreamBuffer *buffToAdd = new StreamBuffer();
	std::pair<std::string, StreamBuffer*> pairToInsert = std::make_pair(key, buffToAdd);
	buffers.insert(pairToInsert);
}

void Streamer::join(){
	pthread_join(streamerThread, NULL);
}
