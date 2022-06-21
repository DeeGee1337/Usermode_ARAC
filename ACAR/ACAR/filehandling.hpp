#pragma once //Anton

#include "globals.hpp"
#define BLACKLIST "BlackList.txt"
#define SEARCHFILE "SearchList.txt"
#define WINDOWTITLE "WindowTitle.txt"
#define OCRLIST "OCRList.txt"

//Anton Halder
bool check_OCRList(std::string ocrcontent) {
	std::fstream file;
	std::string str;
	std::string content;
	file.open(OCRLIST);

	ocrcontent.erase(std::find(ocrcontent.begin(), ocrcontent.end(), '\n'), ocrcontent.end());

	if (!file.is_open())
	{
		std::cerr << "[FILE] Failed to open " << OCRLIST << std::endl;
		return 1;
	}
	else
	{
		while (std::getline(file, str))
		{
			content = str;
			std::size_t found = content.find(ocrcontent);

			std::cout << "[OCRFILECHECK] Within the while " << std::endl;
			std::cout << "[OCRFILECHECK] Printing content: " << content << std::endl;
			if (found != std::string::npos)
			{
				std::cout << "[FILE] OCR match found: " << found << std::endl;
				std::cout << "[FILE] FOUND: " << content << std::endl;
				file.close();
				return 0;
			}
		}
	}
	return 1;
}

//Anton Halder
const std::vector<std::string> search_file() {
	std::fstream file;
	std::string str;
	std::string content;
	std::vector<std::string> searchlist;
	file.open(SEARCHFILE);
	if (!file.is_open())
	{
		std::cerr << "[FILE] Failed to open " << SEARCHFILE << std::endl;
		return searchlist;
	}
	else
	{
		while (std::getline(file, str))
		{
			searchlist.push_back(str);
		}
		return searchlist;
	}
}

//Anton Halder
std::vector<std::wstring> get_blacklisted_windows() {
	std::fstream file;
	std::string str;
	std::string content;
	std::vector<std::wstring> windowlist;
	file.open(WINDOWTITLE);
	if (!file.is_open())
	{
		std::cerr << "[FILE Failed to open " << WINDOWTITLE << std::endl;
		return windowlist;
	}
	else
	{
		while (std::getline(file, str))
		{
			std::wstring strconvert(str.begin(), str.end());
			windowlist.push_back(strconvert);
		}
		return windowlist;
	}

}

//Anton Halder
int open_file(std::string cpu_id, std::vector<std::string> hard_drives) {
	std::fstream file;
	std::string content;
	std::string str;
	int counter = 0;

	file.open(BLACKLIST);
	if (!file.is_open())
	{
		std::cerr << "[FILE] Failed to open " << BLACKLIST << std::endl;
		return -1;
	}
	else
	{
		while (std::getline(file, str))
		{
			content += str;
			std::size_t found = content.find(cpu_id);

			if (found != std::string::npos) {
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
						std::cout << "[FILE] Found string at Startposition: " << found_hd << std::endl;
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

//Anton Halder
int write_file(std::string hwid) {

	std::fstream file;
	std::string hwid_ = hwid;

	file.open(BLACKLIST, std::ios_base::app);	//append modus mit std::ios_base::app
	if (!file.is_open())
	{
		std::cerr << "[FILE] Couldn´t write to Blacklist " << BLACKLIST << std::endl;
		return -1;
	}
	else
	{
		file.write(hwid_.data(), hwid_.size());
		std::cerr << "[FILE] Added HWID to Blacklist" << std::endl;
	}

	return 0;
}

//Anton Halder
std::string create_host_hwid_string(std::string os, std::string cpu_id, std::string hard_drive) {

	std::string hwid = "OS: " + os.append(" CPU: " + cpu_id).append(" Harddrive: " + hard_drive + "\n");

	return hwid;
}

//Anton Halder
std::string create_flagged_hwid_string(std::string cpu_id, std::string hard_drive) {

	std::string hwid = "CPU: " + cpu_id.append(" Harddrive: " + hard_drive + "\n");

	return hwid;
}