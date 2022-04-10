#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <optional>
#include <windows.h>
#include <tuple>
#include <tlhelp32.h>
#include <tchar.h>
#include <winnt.h>
#include <cstring>
#include <algorithm>
#include <comdef.h>
#include <Wbemidl.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <locale.h>
#include <wchar.h>

#include "processes.hpp"
#include "HWID.hpp"
#include "memory.hpp"

namespace sdk
{
    enum class flags
    {
        NONE        = 0,
        Windowname  = 1,
        Memory      = 2,
        Module      = 3,
        RDATA       = 4,
        HWID        = 5
    };

    std::vector<std::tuple<std::wstring, int, sdk::flags>> processes_tuple;
    
    class process
    {
    private:
        std::wstring name;
        int processid;
        sdk::flags inputflag;

    public:
        process(std::wstring inputname, sdk::flags input)
        {
            this->name = inputname;
            this->processid = find_processId(inputname);
            this->inputflag = input;

            processes_tuple.push_back(std::make_tuple(inputname, processid, inputflag));
        };

        int get_id()
        {
            return this->processid;
        }

        sdk::flags get_flag()
        {
            return this->inputflag;
        }

        void set_flag(sdk::flags input)
        {
            std::wcout << "[Memory] Flag changed from: " << (int)this->inputflag << " to " << (int)input << std::endl;
            this->inputflag = input;
        }

        std::wstring get_processname()
        {
            return this->name;
        }
    };
}
