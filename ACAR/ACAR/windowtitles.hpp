#pragma once

#include "globals.hpp"
#include "filehandling.hpp"

//https://stackoverflow.com/questions/10246444/how-can-i-get-enumwindows-to-list-all-windows
//https://docs.microsoft.com/en-gb/windows/win32/api/winuser/nf-winuser-enumwindows?redirectedfrom=MSDN

//Fabian Folger
BOOL CALLBACK safe_window_topmost(HWND hwnd, LPARAM lParam) 
{
    const DWORD TITLE_SIZE = 1024;
    WCHAR windowTitle[TITLE_SIZE];

    GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

    //https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextlengtha
    int length = ::GetWindowTextLength(hwnd);
    std::wstring title(&windowTitle[0]);

    LONG exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

    //Flags for AERO and Topmost overlays
    if (exStyle & WS_EX_TOPMOST && (exStyle & WS_EX_TRANSPARENT || exStyle & WS_EX_LAYERED))
    {
        return TRUE;
    }

    std::vector<std::wstring>& titles = *reinterpret_cast<std::vector<std::wstring>*>(lParam);
    titles.push_back(title);

    return TRUE;
}

//Fabian Folger
BOOL CALLBACK safe_window(HWND hwnd, LPARAM lParam)
{
    const DWORD TITLE_SIZE = 1024;
    WCHAR windowTitle[TITLE_SIZE];

    GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

    int length = ::GetWindowTextLength(hwnd);
    std::wstring title(&windowTitle[0]);

    LONG exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

    //Flags for normal windowstitles
    if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager")
    {
       return TRUE;
    }

    std::vector<std::wstring>& titles = *reinterpret_cast<std::vector<std::wstring>*>(lParam);
    titles.push_back(title);

    return TRUE;
}

//Fabian Folger
bool get_open_window_titles()
{
    std::vector<std::wstring> windows_blacklist = get_blacklisted_windows();

    std::wcout << "[WINDOW] Grabbing all open titles" << std::endl;
    std::vector<std::wstring> titles;
    EnumWindows(safe_window, reinterpret_cast<LPARAM>(&titles));
    // At this point, titles if fully populated and could be displayed, e.g.:
    printf("\n");
    for (const auto& title : titles)
    {
        std::wcout << "[WINDOW] " << L"Title: " << title << std::endl;

        for (auto itt2 : windows_blacklist)
        {
            if (itt2 == title)
            {
                std::wcout << "[WINDOW DETECTION] " << L"Title: " << title  << " FLAG DETECTED!" << std::endl;
                return true;
            }
        }
    }
   
    printf("\n");

    std::wcout << "[WINDOW] Grabbing topmost and aero overlays" << std::endl;
    std::vector<std::wstring> topmosttitles;
    EnumWindows(safe_window_topmost, reinterpret_cast<LPARAM>(&topmosttitles));

    printf("\n");
    for (const auto& title : topmosttitles)
    {
        if (!title.empty())
        {
            std::wcout << "[WINDOW TOPMOST/AERO] " << L"Title: " << title << std::endl;

            for (auto itt2 : windows_blacklist)
            {
                if (itt2 == title)
                {
                    std::wcout << "[WINDOW DETECTION] " << L"Title: " << title << " FLAG DETECTED!" << std::endl;
                    return true;
                }
            }
        }
    }

    printf("\n");
}
