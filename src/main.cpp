/**
 * @file main.cpp
 * @brief 
 * @date 2020-03-10
 * 
 * A  very simple application, which demonstrates 
 * how a gui framework can be built and operated
 * asynchronously
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <windows.h>
#include <CommCtrl.h>
#include "Thread.h"
#include "Button.h"

/**
 * @brief The declaration of the main message processing function
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

ThreadPtr mainThread;           //> The main thread instance 
#define WM_THREAD WM_USER + 1   //> The Windows Message to communicate with the main thread
HINSTANCE hInstance;            //>The main instance handle, used for the create of every window in process

/**
 * @brief The beginning of everything
 * 
 * @param hInstance         //> The instance handle, to be contacted, for example, by other processes 
 * @param hPrevInstance     //> The handle from the parent process
 * @param lpCmdLine         //> The command line with all arguments passed
 * @param nCmdShow          //> The display code , it determines how the application window is initially displayed  
 * @return int WinMain      //> The return value of the application. Traditionally zero if everything was ok
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ::hInstance = hInstance; // store the instance handle in the global variable

    MSG msg = {0};
    WNDCLASS wc = {0};    
    wc.lpfnWndProc = WndProc;   // Announcing the main message processing function
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
                               (sx - 640) / 2,  // center the window on screen
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

HWND hwndPB = NULL;
void OnStartClicked(Button* button);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        ButtonPtr start = Button::Manager["Start:Button"];
        start->OnClicked = OnStartClicked;
        start->create(hWnd, 20, 20, 100, 30, "Start ...");

        RECT rcClient;
        ::GetClientRect(hWnd, &rcClient);

        int cyVScroll = GetSystemMetrics(SM_CYVSCROLL);

        ::hwndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
                                 WS_CHILD | WS_VISIBLE, rcClient.left,
                                 rcClient.bottom - cyVScroll * 2,
                                 rcClient.right, cyVScroll * 2,
                                 hWnd, (HMENU)0, ::hInstance, NULL);

        // SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, 100));

        // SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
  }
    break;

    case WM_COMMAND:
    {
        HWND hCtrl = reinterpret_cast<HWND>(lParam);
        if (HIWORD(wParam) == BN_CLICKED)
        {
            ButtonPtr button = Button::Manager[hCtrl];
            button->execute(BN_CLICKED);
        }
    }
    break;

    case WM_KEYUP:
    {
        if(wParam  == VK_ESCAPE)
            ::PostMessage(hWnd, WM_CLOSE, 0, 0);
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

void OnStartClicked(Button *button)
{
    ThreadPtr thread1 = Thread::Manager["Thread1"];
    thread1->start();
    thread1->call([thread1]() {
        for(int i = 0; i < 10; ++i)
        {
            ::Sleep(800);
            ::PostMessage(hwndPB, PBM_STEPIT, 0, 0);
        }

        mainThread->call([thread1](){
            thread1->stop();
        });
    });
}