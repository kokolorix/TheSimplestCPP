/**
 * @file WinApp.cpp
 * @brief 
 * @date 2020-03-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <windows.h>

#include "Thread.h"
# include "Button.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

ThreadPtr mainThread;
#define WM_THREAD WM_USER + 1

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg = {0};
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = L"WinApp";

    if (!::RegisterClass(&wc))
        return 1;

    int sx = ::GetSystemMetrics(SM_CXSCREEN);
    int sy = ::GetSystemMetrics(SM_CYSCREEN);

    HWND hWnd = ::CreateWindow(wc.lpszClassName,
                               L"WinApp",
                               WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               (sx - 640) / 2,
                               (sy - 480) / 2,
                               640,
                               480,
                               0,
                               0,
                               hInstance,
                               NULL);

    if (!hWnd)
        return 2;

    mainThread = Thread::Manager["MainThread"];
    mainThread->Notify = [hWnd]() { ::PostMessage(hWnd, WM_THREAD, (WPARAM)0, (LPARAM)0); };

    while (::GetMessage(&msg, NULL, 0, 0) > 0)
    {
        ::DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        ButtonPtr start = Button::Manager["Start:Button"];
        HWND hStart = start->create(hWnd, 20, 20, 30, 100);
    }
    break;

    case WM_CLOSE:
        ::PostQuitMessage(0);
        break;

    case WM_THREAD:
        mainThread->processQueue();
        break;

    default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}