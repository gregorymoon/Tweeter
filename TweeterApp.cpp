#include <cstdio>
#include <string>
#include <dirent.h>

#include "User.h"
#include "Streamer.h"

void readAllUsersFromFile(std::string dirPath);
void readUserFromPath(std::string filepath);

int main(int argc, char* argv[]){
	//get dir location from user input
	std::string dirPath;

	if(argc == 2)
		dirPath = argv[1];
	else
		 dirPath = "/home/grmoon/school/cse430/projects/2/sample_input/";
	
	readAllUsersFromFile(dirPath);

	//Streamer::start();

	//User *u1 = new User("handle");

	//Streamer::join();
}

void readAllUsersFromFile(std::string dirPath){
	DIR *dir = opendir(dirPath.c_str());

	if(!dir){
		perror("opendir");
		exit(1);
	}
	else{
		struct dirent *dirEntry;
		while((dirEntry = readdir(dir)) != NULL){
			printf("%s\n", dirEntry->d_name);
		}
	}		
}

void readUserFromPath(std::string filepath){

}

