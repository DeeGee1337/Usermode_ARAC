#pragma once

#include "globals.hpp"

#define IWVTU_FULL    1
#define IWVTU_PARTIAL 2
#define IWVTU_HIDDEN  3
#define IWVTU_ERROR   4

//http://bobmoore.mvps.org/Win32/w32tip72.htm
//because topmost check was buggy

//Christoph Sommer, Anton Halder, Fabian Folger
UINT is_window_visible_to_user(HWND hWnd)
{
    std::cout << "[GAME DECTECTOR] Cheking if window is visible to user..." << std::endl;

    UINT uRetVal = IWVTU_ERROR;
    int  iClipRes;
    HDC hdcWnd;
    RECT rcWndClient;
    RECT rcWndClip;

    if (IsWindow(hWnd))
    {
        // This uses the clipping system to determine if the user
        // can see all or part of the window. The only problem
        // comes when the region returned is a rectangle, i.e. the
        // return code from GetClipBox is SIMPLEREGION. In that
        // case we check against the client area rectangle, on the
        // basis that what we _really_ care about is the contents
        // of the window, not the caption bar.

        hdcWnd = GetDC(hWnd);
        GetClientRect(hWnd, &rcWndClient);
        iClipRes = GetClipBox(hdcWnd, &rcWndClip);
        ReleaseDC(hWnd, hdcWnd);

        switch (iClipRes)
        {
        case NULLREGION:
            uRetVal = IWVTU_HIDDEN;
            break;
        case SIMPLEREGION:
            if (EqualRect(&rcWndClip, &rcWndClient))
                uRetVal = IWVTU_FULL;
            else
                uRetVal = IWVTU_PARTIAL;
            break;
        case COMPLEXREGION:
            uRetVal = IWVTU_PARTIAL;
            break;
        default:
            uRetVal = IWVTU_ERROR;
            break;
        }
    }
    return uRetVal;
}

//Christoph Sommer, Anton Halder, Fabian Folger
class margins
{
private:
    int left_with;
    int right_with;
    int top_hight;
    int bottom_height;

public:
    margins()
    {
        this->left_with = 0;
        this->right_with = 0;
        this->top_hight = 0;
        this->bottom_height = 0;
    }
    margins(int left_with, int right_with, int top_hight, int bottom_height)
    {
        this->left_with = left_with;
        this->right_with = right_with;
        this->top_hight = top_hight;
        this->bottom_height = bottom_height;
    }

    void set_left_with(int input)
    {
        this->left_with = input;
    }

    void set_right_with(int input)
    {
        this->right_with = input;
    }

    void set_top_hight(int input)
    {
        this->top_hight = input;
    }

    void set_bottom_height(int input)
    {
        this->bottom_height = input;
    }

    int get_left_with()
    {
        return this->left_with;
    }

    int get_right_with()
    {
        return this->right_with;
    }

    int get_top_hight()
    {
        return this->top_hight;
    }

    int get_bottom_height()
    {
        return this->bottom_height;
    }
};

//Christoph Sommer, Anton Halder, Fabian Folger
margins find_game_window_and_resolution(std::wstring process)
{
    margins gamewindow;
	std::wcout << "[GAME DECTECTOR] Looking for process: " << process << std::endl;
	auto process_id = find_processId(process);
	std::cout << "[GAME DECTECTOR] Processwindow found: " << process_id << std::endl;

	auto process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
	if (process_handle == nullptr)
	{
		std::cout << "[GAME DECTECTOR] Processhandle not found! " << std::endl;
		return gamewindow;
	}

	auto window = FindWindowA("Valve001", "Counter-Strike: Global Offensive - Direct3D 9");
	if (window)
	{
        std::cout << "[GAME DECTECTOR] Gamewindow found!" << std::endl;
        printf("[GAME DECTECTOR] CSGO wnd: 0x%p\n", window);

        auto output = is_window_visible_to_user(window);
        std::cout << "[GAME DECTECTOR] Windowcheck: " << output << std::endl;

        //is window vis to user?
        if (output == IWVTU_FULL)
        {
            std::cout << "[GAME DECTECTOR] Gamewindow is TOPMOST!" << std::endl;
            RECT client{}, window_rect{};
            GetWindowRect(window, &window_rect);
            GetClientRect(window, &client);

            POINT diff{};
            ClientToScreen(window, &diff);

            auto left_border = window_rect.left + (diff.x - window_rect.left);
            auto right_border = window_rect.top + (diff.y - window_rect.top);
            auto height_top_border = client.right;
            auto height_bottom_border = client.bottom;

            std::cout << "[GAME DECTECTOR] left_border:          " << left_border << std::endl;
            std::cout << "[GAME DECTECTOR] right_border:         " << right_border << std::endl;
            std::cout << "[GAME DECTECTOR] height_top_border:    " << height_top_border << std::endl;
            std::cout << "[GAME DECTECTOR] height_bottom_border: " << height_bottom_border << std::endl;

            gamewindow.set_left_with(left_border);
            gamewindow.set_right_with(right_border);
            gamewindow.set_top_hight(height_top_border);
            gamewindow.set_bottom_height(height_bottom_border);

            return gamewindow;
        }
	}

	return gamewindow;
}