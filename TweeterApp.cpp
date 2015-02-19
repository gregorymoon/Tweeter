#include <cstdio>
#include <string>
#include <dirent.h>
#include <cstring>
#include <iostream>
#include <fstream>

#include "User.h"
#include "Streamer.h"

void readAllUsersFromFile(std::string dirPath);
void readUserFromFile(std::string filepath);

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
			if(strstr(dirEntry->d_name, ".txt") != NULL){
				std::string filepath = dirPath + dirEntry->d_name;
				printf("Reading User from %s\n", dirEntry->d_name);
				readUserFromFile(filepath);
			}
		}
	
		closedir(dir);
	}		
}

void readUserFromFile(std::string filepath){
	std::ifstream currFile;
	currFile.open(filepath);

	if(currFile.is_open()){
		std::string currLine;

		while(std::getline(currFile, currLine)){
			printf("%s\n", currLine.c_str());
		}
	}
	else{
		perror("openfile");
		exit(1);
	}

	currFile.close();
}

