#pragma once

#include "globals.hpp"

namespace memory
{
    int detect_modules(DWORD processID)
    {
        auto results = 0;
        HMODULE hMods[1024];
        HANDLE hProcess;
        DWORD cbNeeded;
        unsigned int i;
        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
        if (NULL == hProcess)
            return 1;

        if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
        {
            for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
            {
                TCHAR szModName[MAX_PATH];

                if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
                {
                    if (wcsstr(szModName, L"lua53-32.dll") || wcsstr(szModName, L"lua53-64.dll") || wcsstr(szModName, L"lfs.dll")) //Cheat Engine Detection
                    {
                        std::wcout << "\t" << szModName << std::endl;
                        results++;
                    }

                    if (wcsstr(szModName, L"NativeHelper.dll") || wcsstr(szModName, L"ReClass.NET.exe")) //Reclass Detection
                    {
                        std::wcout << "\t" << szModName << std::endl;
                        results++;
                    }

                    if (wcsstr(szModName, L"ida64.dll") || wcsstr(szModName, L"ida64.exe")) //IDA Detection
                    {
                        std::wcout << "\t" << szModName << std::endl;
                        results++;
                    }
                }
            }
        }
        CloseHandle(hProcess);
        return results;
    }

    int search_for_string(DWORD pid, const char* data, size_t len)
    {
        int results = 0;
        HANDLE process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (process)
        {
            SYSTEM_INFO si;
            GetSystemInfo(&si);

            MEMORY_BASIC_INFORMATION info;
            char* buff = NULL;
            char* p = 0;
            while (p < si.lpMaximumApplicationAddress)
            {
                if (VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info))
                {
                    if (info.State != MEM_FREE) {
                        //std::cout << std::hex << info.BaseAddress << " | " << std::hex << info.RegionSize << std::endl;
                        p = (char*)info.BaseAddress;
                        buff = (char*)new char[info.RegionSize];
                        SIZE_T bytesRead;
                        if (ReadProcessMemory(process, p, &buff[0], info.RegionSize, &bytesRead))
                        {
                            for (size_t i = 0; i < (bytesRead - len); ++i)
                            {
                                if (memcmp(data, &buff[i], len) == 0)
                                {
                                    results++;
                                    std::cout << "\t" << std::hex << (void*)(p + i) << "\t" << data << std::endl;
                                    //return (char*)p + i;
                                }
                            }
                        }
                        free(buff);
                    }
                    p += info.RegionSize;
                }
            }
        }
        return results;
    }
}