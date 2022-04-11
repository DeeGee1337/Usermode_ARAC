#pragma once

#include "globals.hpp"
#define TEXTFILE "SearchList.txt"

int write_file(std::string, std::string, std::string);


int create_file(std::string os, std::string cpu_id, std::string hard_drive) {
	std::ofstream file;
	file.open(TEXTFILE);
	if (!file.is_open()) 
	{
		std::cerr << "[FILE] Failed to open " << TEXTFILE << std::endl;
		return -1;
	}
	else
	{
		write_file(os, cpu_id, hard_drive);
	}
	file.close();
	std::cout << "[FILE] Created File" << std::endl;
	return 0;
}

int write_file(std::string os, std::string cpu_id, std::string hard_drive) {

	std::fstream file;
	std::string hwid = "OS: " + os.append(" CPU: " + cpu_id).append(" Harddrive: " + hard_drive + "\n");

	file.open(TEXTFILE, std::ios_base::app);	//append modus mit std::ios_base::app
	{	
		file.write(hwid.data(), hwid.size());
		std::cerr << "[FILE] Done writing" << std::endl;
	}

	return 0;
}