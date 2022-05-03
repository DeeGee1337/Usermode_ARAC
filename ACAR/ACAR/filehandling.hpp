#pragma once

#include "globals.hpp"
#define TEXTFILE "BlackList.txt"

int write_file(std::string, std::string);


int open_file(std::string cpu_id, std::vector<std::string> hard_drives) {
	std::fstream file;
	std::string content;
	std::string str;
	int counter = 0;

	file.open(TEXTFILE);
	if (!file.is_open()) 
	{
		std::cerr << "[FILE] Failed to open " << TEXTFILE << std::endl;
		return -1;
	}
	else
	{
		while (std::getline(file, str))
		{
			counter++;
			content += str;
			std::size_t found = content.find(cpu_id);

			if (found!=std::string::npos){
				std::cout << "[FILE] Printing found: " << found << std::endl;
				std::cout << "[FILE] HWID already blacklisted" << std::endl;
				file.close();
				std::cout << "[FILE] Printing CPU from Blacklist: " << content << std::endl;
				return -2;
			}
			else {
				for (const auto itt : hard_drives) {
					std::size_t found_hd = str.find(itt);
					if (found_hd != std::string::npos) {
						std::cout << "[FILE] HWID already blacklisted" << std::endl;
						file.close();
						std::cout << "[FILE] Printing CPU from Blacklist: " << content << std::endl;
						return -2;
					}
				}
			}
			
		}
	}
	file.close();
	std::cout << "[FILE] Blacklist check done" << std::endl;
	return 0;
}

int write_file(std::string hwid) {

	std::fstream file;
	std::string hwid_ = hwid;
	
	file.open(TEXTFILE, std::ios_base::app);	//append modus mit std::ios_base::app
	if (!file.is_open())
	{
		std::cerr << "[FILE] Couldn´t write to Blacklist " << TEXTFILE << std::endl;
		return -1;
	}
	else
	{	
		file.write(hwid_.data(), hwid_.size());
		std::cerr << "[FILE] Added HWID to Blacklist" << std::endl;
	}

	return 0;
}

std::string create_host_hwid_string(std::string os, std::string cpu_id, std::string hard_drive) {

	std::string hwid = "OS: " + os.append(" CPU: " + cpu_id).append(" Harddrive: " + hard_drive + "\n");
	
	return hwid;
}

std::string create_flagged_hwid_string(std::string cpu_id, std::string hard_drive) {

	std::string hwid = "CPU: " + cpu_id.append(" Harddrive: " + hard_drive + "\n");

	return hwid;
}