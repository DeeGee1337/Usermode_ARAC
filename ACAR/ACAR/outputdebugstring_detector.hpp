#pragma once

#include "globals.hpp"

//Because some Programs with only GUI use OutputDebugString
//https://docs.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-outputdebugstringa
//https://www.codeproject.com/Articles/23776/Mechanism-of-OutputDebugString
// Programm to read:
//https://docs.microsoft.com/en-us/sysinternals/downloads/debugview

struct dbwin_buffer
{
	DWORD   dwProcessId;
	char    data[4096 - sizeof(DWORD)];
};

std::vector<std::string>blacklisted_debug_outputs =
{
	"Starting CE", //On startup
	"calling peinfo_getEntryPoint", //On new attach
	"calling peinfo_getdatabase", //On new attach
	"getProcessPathFromProcessID", //On new attach
	"TSavedScanHandler.InitializeScanHandler", //On memory scan

	//IDA
	"NVD3DREL:"
};

bool debug_string_detection()
{
	std::cout << "[OUTPUTDEBUG DETECTOR] Starting..." << std::endl;

	HANDLE hmutex = OpenMutexA(MUTEX_ALL_ACCESS, 0, "DBWinMutex");
	HANDLE hevent_buffer_ready = OpenEventA(EVENT_ALL_ACCESS, 0, "DBWIN_BUFFER_READY");

	if (hevent_buffer_ready == NULL) 
	{
		hevent_buffer_ready = CreateEventA(NULL, FALSE, TRUE, "DBWIN_BUFFER_READY");
		if (!hevent_buffer_ready)
		{
			std::cout << "[OUTPUTDEBUG DETECTOR] Couldn't create DBWIN_BUFFER_READY" << std::endl;
			return false;
		}
	}

	HANDLE hevent_data_ready = OpenEventA(SYNCHRONIZE, 0, "DBWIN_DATA_READY");

	if (hevent_data_ready == NULL) 
	{
		hevent_data_ready = CreateEventA(NULL, FALSE, FALSE, "DBWIN_DATA_READY");
		if (!hevent_data_ready)
		{
			std::cout << "[OUTPUTDEBUG DETECTOR] Couldn't create DBWIN_DATA_READY" << std::endl;
			return false;
		}
	}

	HANDLE file_mapping = OpenFileMappingA(FILE_MAP_READ, FALSE, "DBWIN_BUFFER");
	
	if (file_mapping == NULL) 
	{
		file_mapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(dbwin_buffer), "DBWIN_BUFFER");
		if (!file_mapping)
		{
			std::cout << "[OUTPUTDEBUG DETECTOR] Couldn't create DBWIN_BUFFER" << std::endl;
			return false;
		}
	}

	dbwin_buffer* buffer = (dbwin_buffer*)MapViewOfFile(file_mapping, SECTION_MAP_READ, 0, 0, 0);

	if (!buffer)
	{
		std::cout << "[OUTPUTDEBUG DETECTOR] Couldn't create buffer" << std::endl;
		return false;
	}

	std::cout << "[OUTPUTDEBUG DETECTOR] While True" << std::endl; // TODO -> Spawn a thread here

	while (true)
	{
		bool breakval = false;
		DWORD ret = WaitForSingleObject(hevent_data_ready, 1);

		if (ret == WAIT_OBJECT_0) 
		{
			for (auto blacklisted_debug_output : blacklisted_debug_outputs) 
			{
				if (std::string(buffer->data).find(blacklisted_debug_output) != std::string::npos) 
				{
					printf("[OUTPUTDEBUG DETECTOR] Blacklisted output: %s\n", buffer->data);
					return true;
				}
			}
			SetEvent(hevent_buffer_ready);
			break;
		}
	}

	return false;
}