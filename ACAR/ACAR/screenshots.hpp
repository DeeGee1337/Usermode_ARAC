//bitblt screenshot 400x400 	time : [6.9263 ms 6.9439 ms 6.9613 ms]
//dxgi screenshot 400x400 	time : [108.45 ms 343.04 ms 662.41 ms]

//https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-bitblt

#pragma once

#include "globals.hpp"
#include "game_window_detector.hpp"

//Christoph Sommer, Anton Halder, Fabian Folger
bool save_bitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal)
{
    bool result = false;
    PICTDESC pd;

    pd.cbSizeofstruct = sizeof(PICTDESC);
    pd.picType = PICTYPE_BITMAP;
    pd.bmp.hbitmap = bmp;
    pd.bmp.hpal = pal;

    LPPICTURE picture;
    HRESULT res = OleCreatePictureIndirect(&pd, IID_IPicture, false,
        reinterpret_cast<void**>(&picture));

    if (!SUCCEEDED(res))
        return false;

    LPSTREAM stream;
    res = CreateStreamOnHGlobal(0, true, &stream);

    if (!SUCCEEDED(res))
    {
        picture->Release();
        return false;
    }

    LONG bytes_streamed;
    res = picture->SaveAsFile(stream, true, &bytes_streamed);

    HANDLE file = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_READ, 0, //https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    if (!SUCCEEDED(res) || !file)
    {
        stream->Release();
        picture->Release();
        return false;
    }

    HGLOBAL mem = 0;
    GetHGlobalFromStream(stream, &mem);
    LPVOID data = GlobalLock(mem);

    DWORD bytes_written;

    result = !!WriteFile(file, data, bytes_streamed, &bytes_written, 0);
    result &= (bytes_written == static_cast<DWORD>(bytes_streamed));

    GlobalUnlock(mem);
    CloseHandle(file);

    stream->Release();
    picture->Release();

    return result;
}

//Christoph Sommer, Anton Halder, Fabian Folger
bool screen_capture_part(margins inputwindow, LPCSTR fname) 
{
    HDC hdcSource = GetDC(NULL);
    HDC hdcMemory = CreateCompatibleDC(hdcSource);

    int capX = GetDeviceCaps(hdcSource, HORZRES);
    int capY = GetDeviceCaps(hdcSource, VERTRES);

    HBITMAP hBitmap = CreateCompatibleBitmap(hdcSource, inputwindow.get_top_hight(), inputwindow.get_bottom_height());
    HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMemory, hBitmap);

    BitBlt(hdcMemory, 0, 0, inputwindow.get_top_hight(), inputwindow.get_bottom_height(), hdcSource, inputwindow.get_left_with(), inputwindow.get_left_with(), SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hdcMemory, hBitmapOld);

    DeleteDC(hdcSource);
    DeleteDC(hdcMemory);

    HPALETTE hpal = NULL;
    if (save_bitmap(fname, hBitmap, hpal)) return true;
    return false;
}