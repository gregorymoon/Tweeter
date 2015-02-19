#include <cstdio>
#include <string>

#include "User.h"
#include "Streamer.h"

int main(){
	Streamer::start();

	User *u1 = new User("handle");

	Streamer::join();
}

