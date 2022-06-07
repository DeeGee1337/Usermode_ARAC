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
#include <cctype>
#include <mutex>



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
        NONE = 0,
        Process = 1,
        Windowname = 2,
        OCR = 3,
        HWID = 4,
        Memory = 5,
        Module = 6
    };

    std::string wstring_to_string(std::wstring input)
    {
        std::string str(input.begin(), input.end());
        return str;
    }

    std::vector<std::tuple<std::wstring, int>> processes_tuple;

    class host
    {
    private:
        std::string hwid;
        sdk::flags status = sdk::flags::NONE;

    public:
        host(std::string input)
        {
            this->hwid = input;
            std::cout << "[HOST] HWID added to class: " << this->hwid << std::endl;
        }

        sdk::flags get_flag()
        {
            return this->status;
        }

        void set_flag(sdk::flags input)
        {
            if (input > this->status)
            {
                std::wcout << "[HOST] Flag changed from: " << (int)this->status << " to " << (int)input << std::endl;
                this->status = input;
            }
            else
            {
                std::wcout << "[HOST] Flag is already higher" << std::endl;
            }
        }
    };

    class process
    {
    private:
        std::wstring name;
        int processid;

    public:
        process(std::wstring inputname)
        {
            this->name = inputname;
            this->processid = find_processId(inputname);

            processes_tuple.push_back(std::make_tuple(inputname, processid));
        };

        int get_id()
        {
            return this->processid;
        }

        std::wstring get_processname()
        {
            return this->name;
        }
    };
}