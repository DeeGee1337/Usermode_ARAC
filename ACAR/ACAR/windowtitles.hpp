#pragma once

#include "globals.hpp"

//Fabian Folger
BOOL CALLBACK safe_window_topmost(HWND hwnd, LPARAM lParam) 
{
    const DWORD TITLE_SIZE = 1024;
    WCHAR windowTitle[TITLE_SIZE];

    GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

    int length = ::GetWindowTextLength(hwnd);
    std::wstring title(&windowTitle[0]);

    LONG exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

    if (exStyle & WS_EX_TOPMOST && (exStyle & WS_EX_TRANSPARENT || exStyle & WS_EX_LAYERED))
    {
        return TRUE; //topmost and aero + transparent
    }

    // Retrieve the pointer passed into this callback, and re-'type' it.
    // The only way for a C API to pass arbitrary data is by means of a void*.
    std::vector<std::wstring>& titles =
        *reinterpret_cast<std::vector<std::wstring>*>(lParam);
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

    if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager")
    {
       return TRUE; //all open wiondows
    }

    // Retrieve the pointer passed into this callback, and re-'type' it.
    // The only way for a C API to pass arbitrary data is by means of a void*.
    std::vector<std::wstring>& titles =
        *reinterpret_cast<std::vector<std::wstring>*>(lParam);
    titles.push_back(title);

    return TRUE;
}

//Fabian Folger
bool get_open_window_titles()
{
    std::vector<std::wstring> windows_blacklist;
    windows_blacklist.push_back(L"AeroOverlay dx");


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
