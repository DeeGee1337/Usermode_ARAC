#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
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
#include <ole2.h>
#include <olectl.h>
#include <windef.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <thread>

#include "processes.hpp"
#include "HWID.hpp"
#include "memory.hpp"
#include "windowtitles.hpp"
#include "filehandling.hpp"
#include "patternscanner.hpp"
#include "outputdebugstring_detector.hpp"
#include "screenshots.hpp"
#include "game_window_detector.hpp"
#include "tesseract_ocr.hpp"

namespace sdk
{
    enum class flags
    {
        NONE        = 0,
        Process     = 1,
        Windowname  = 2,
        HWID        = 3,
        Memory      = 4,
        Module      = 5,
        RDATA       = 6
    };

    std::string wstring_to_string(std::wstring input)
    {
        std::string str(input.begin(), input.end());
        return str;
    }

    std::vector<std::tuple<std::wstring, int, sdk::flags>> processes_tuple;
    
    class process
    {
    private:
        std::wstring name;
        int processid;
        sdk::flags inputflag = sdk::flags::NONE;

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
            if (input > this->inputflag)
            {
                std::wcout << "[Memory] Flag changed from: " << (int)this->inputflag << " to " << (int)input << std::endl;
                this->inputflag = input;
            }
            else
            {
                std::wcout << "[Memory] Flag is higher" << std::endl;
            }
        }

        std::wstring get_processname()
        {
            return this->name;
        }
    };
}

