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

#include "processes.hpp"

namespace sdk
{
    enum class flags
    {
        NONE        = 0,
        Windowname  = 1,
        Memor       = 2,
        Module      = 3,
        RDATA       = 4,
        HWID        = 5
    };

    std::vector<std::tuple<std::wstring, int, sdk::flags>> processes_tuple;
    
    class process
    {
    public:
        process(std::wstring inputname, sdk::flags input)
        {
            this->name = inputname;
            this->processid = find_processId(inputname);
            this->inputflag = input;

            processes_tuple.push_back(std::make_tuple(inputname, processid, inputflag));
        };


    private:
        std::wstring name;
        int processid;
        sdk::flags inputflag;
    };
}
