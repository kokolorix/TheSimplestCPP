/**
 * @file WinApp.cpp
 * @brief 
 * @date 2020-03-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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

    if (!::CreateWindow(wc.lpszClassName,
                      L"WinApp",
                      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                      (sx - 640 ) / 2,
                      (sy -480) / 2,
                      640,
                      480,
                      0,
                      0,
                      hInstance,
                      NULL))
        return 2;

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
    case WM_CLOSE:
        ::PostQuitMessage(0);
        break;
    default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}