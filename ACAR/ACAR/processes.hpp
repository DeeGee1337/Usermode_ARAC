#pragma once

#include "globals.hpp"

std::wstring print_processes(std::wstring searchinput)
{
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        std::wcout << "[DEBUG] Failed to get Snapshot" << std::endl;
        return L"0";
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        std::wcout << "[DEBUG] Get Process32First failed" << std::endl; // show cause of failure
        CloseHandle(hProcessSnap);          // clean the snapshot object
        return L"0";
    }

    // Now walk the snapshot of processes, and
    // display information about each process in turn
    do
    {
        std::wcout << "\n\n=====================================================" << std::endl;
        std::wcout << "\nPROCESS NAME: " << pe32.szExeFile << std::endl; //NEEDE BECAUSE OF UNICODE !!!!! https://stackoverflow.com/questions/37787274/szexefile-in-processentery32-stucture-gives-a-weird-value
        std::wcout << "\n\n=====================================================" << std::endl;

        if (pe32.szExeFile)
        {
            std::wstring current = pe32.szExeFile;
            
            if (current.find(searchinput) != std::string::npos)
            {
                std::wcout << "[DEBUG] Substring found in Proccesslist: " << current << " | Input: " << searchinput << std::endl;
                return current;
            }

            std::transform(current.begin(), current.end(), current.begin(),[](unsigned char c) { return std::tolower(c); }); //lmao hacky
            std::wcout << "[DEBUG] std::transform: " << current << std::endl;

            if (current.find(searchinput) != std::string::npos)
            {
                std::wcout << "[DEBUG] TOLOWER -> Substring found in Proccesslist: " << current << " | Input: " << searchinput << std::endl;
                return current;
            }
        }

        // Retrieve the priority class.
        dwPriorityClass = 0;
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

        if (hProcess == NULL)
            std::cout << "OpenProcess" << std::endl;
        else
        {
            dwPriorityClass = GetPriorityClass(hProcess);
            if (!dwPriorityClass)
                std::cout << "GetPriorityClass" << std::endl;
            CloseHandle(hProcess);
        }

        printf("\n  Process ID        = 0x%08X", pe32.th32ProcessID);
        printf("\n  Thread count      = %d", pe32.cntThreads);
        printf("\n  Parent process ID = 0x%08X", pe32.th32ParentProcessID);
        printf("\n  Priority base     = %d", pe32.pcPriClassBase);

        if (dwPriorityClass)
            printf("\n  Priority class    = %d", dwPriorityClass);

        //TODO
        // List the modules and threads associated with this process
        //ListProcessModules(pe32.th32ProcessID);
        // ListProcessThreads(pe32.th32ProcessID);

    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    printf("\n\n");
    return L"0";
}

DWORD find_processId(const std::wstring& processName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    std::wcout << "[DEBUG] find_processId: " << processName << std::endl;

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile))
    {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!processName.compare(processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}

void kill_process_by_name(const std::wstring& processName)
{
    DWORD dwProcessId = find_processId(processName);
    HANDLE  hProcess;
    BOOL    fResult = FALSE;

    std::wcout << "[DEBUG] kill_process_by_name: " << processName << std::endl;

    if (dwProcessId == 0) 
    {
        printf("[DEBUG] failed to get process id for %s\n", processName);
    }

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

    if (hProcess != NULL) 
    {
        printf("[DEBUG] going to terminate process: %ld\n", dwProcessId);
        fResult = TerminateProcess(hProcess, 0);
        WaitForSingleObject(hProcess, 500);
        CloseHandle(hProcess);
    }
}

void kill_process_by_ID(DWORD input_ID)
{
    DWORD dwProcessId = input_ID;
    HANDLE  hProcess;
    BOOL    fResult = FALSE;

    std::wcout << "[DEBUG] kill_process_by_ID: " << input_ID << std::endl;

    if (dwProcessId == 0)
    {
        printf("[DEBUG] failed to get process id for %i\n", input_ID);
    }

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

    if (hProcess != NULL)
    {
        printf("[DEBUG] going to terminate process: %ld\n", dwProcessId);
        fResult = TerminateProcess(hProcess, 0);
        WaitForSingleObject(hProcess, 500);
        CloseHandle(hProcess);
    }
}
