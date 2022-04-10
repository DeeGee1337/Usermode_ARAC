#pragma once

#include "globals.hpp"

BOOL CALLBACK safe_window(HWND hwnd, LPARAM lParam) 
{
    const DWORD TITLE_SIZE = 1024;
    WCHAR windowTitle[TITLE_SIZE];

    GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

    int length = ::GetWindowTextLength(hwnd);
    std::wstring title(&windowTitle[0]);

    if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager") 
    {
        return TRUE;
    }

    // Retrieve the pointer passed into this callback, and re-'type' it.
    // The only way for a C API to pass arbitrary data is by means of a void*.
    std::vector<std::wstring>& titles =
        *reinterpret_cast<std::vector<std::wstring>*>(lParam);
    titles.push_back(title);

    return TRUE;
}

void get_open_window_titles()
{
    std::vector<std::wstring> titles;
    EnumWindows(safe_window, reinterpret_cast<LPARAM>(&titles));
    // At this point, titles if fully populated and could be displayed, e.g.:
    printf("\n");
    for (const auto& title : titles)
        std::wcout << "[WINDOW] " << L"Title: " << title << std::endl;
   
    printf("\n");
}