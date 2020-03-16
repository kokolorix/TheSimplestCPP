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
#include <sstream>
using std::ostringstream;
using std::endl;

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
    mainThread->initRunningThread(this_thread::get_id(), [hWnd]() {
         ::PostMessage(hWnd, WM_THREAD, (WPARAM)0, (LPARAM)0); 
         });

    while (::GetMessage(&msg, NULL, 0, 0) > 0)
    {
        ::DispatchMessage(&msg);
    }

    return 0;

}

HWND hWndPB = NULL;
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

        ::hWndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
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

    case WM_LBUTTONDOWN:
        ::SetFocus(hWnd);
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

/**
 * @brief All the magic is here :-)
 * 
 * @msc
 * hscale="2.5", arcgradient="10";
 * gui[linecolor="violet"], tr1[linecolor="olive"], tr2[linecolor="orange"];
 * 
 * |||;
 * 
 * gui box gui[label="GUI-Thread", textbgcolour="violet"];
 * tr1 box tr1[label="Worker1", textbgcolour="olive"];
 * tr2 box tr2[label="Worker2", textbgcolour="orange"];
 * 
 * gui => tr1 [label="start()",linecolor="violet"];
 * gui => tr2 [label="start()",linecolor="violet"];
 * 
 * gui => tr2 [label="call(...)",linecolor="violet"];
 * 
 * gui => tr1 [label="call(...)",linecolor="violet"];
 * 
 * tr2 => tr2 [label="sleep(300)",linecolor="orange", arcskip="1"];
 * tr2 => gui [label="button->caption='Running |'",linecolor="orange"];
 * tr2 => tr2 [label="sleep(300)",linecolor="orange", arcskip="1"];
 * tr2 => gui [label="button->caption='Running /'",linecolor="orange"];
 * tr2 => tr2 [label="sleep(300)",linecolor="orange", arcskip="1"];
 * tr2 => gui [label="button->caption='Running -'",linecolor="orange"];
 * 
 * tr1 => tr1 [label="sleep(900)",linecolor="olive", arcskip="1"];
 * tr1 => gui [label="PostMessage(hWndPB, PBM_STEPIT, 0, 0)",linecolor="olive"];
 * 
 * tr2 => tr2 [label="sleep(300)",linecolor="orange", arcskip="1"];
 * tr2 => gui [label="button->caption='Running \'",linecolor="orange"];
 * tr2 => tr2 [label="sleep(300)",linecolor="orange", arcskip="1"];
 * tr2 => gui [label="button->caption='Running |'",linecolor="orange"];
 * tr2 => tr2 [label="sleep(300)",linecolor="orange", arcskip="1"];
 * tr2 => gui [label="button->caption='Running /'",linecolor="orange"];
 * 
 * 
 * tr1 => tr1 [label="sleep(900)",linecolor="olive", arcskip="1"];
 * tr1 => gui [label="PostMessage(hWndPB, PBM_STEPIT, 0, 0)",linecolor="olive"];
 * 
 * ---;
 * 
 * tr2 => gui [label="button->caption='Start...'",linecolor="orange"];
 * gui => tr1 [label="stop()",linecolor="violet"];
 * gui => tr2 [label="stop()",linecolor="violet"];
 * @endmsc
 * 
 * @param button 
 */
void OnStartClicked(Button *button)
{
    // First, we fetch the first worker thread and start it when needed
    ThreadPtr thread1 = Thread::Manager["Thread1"];
    if(!thread1->IsRunning)
        thread1->start();

    
    // Then we get the second worker thread and start it if necessary
    ThreadPtr thread2 = Thread::Manager["Thread2"];
    if(!thread2->IsRunning)
        thread2->start();

    // Then we send the job to the second worker thread every 3 tenths of a second
    // update the text of the start button
    thread2->call([thread2]() {
        while(!thread2->IsStopped)
        {
            ::Sleep(300);
            mainThread->call([thread2]() {
                static int count = 0;
                string progress[] = {"|","/", "-", "\\" };
                int size = sizeof(progress) / sizeof(string);
                ButtonPtr button = Button::Manager["Start:Button"];
                button->Caption = "Running " + progress[count % size];
                count++;
            });
        }
        // If the thread was stopped, we reset the text
        mainThread->call([](){
             ButtonPtr button = Button::Manager["Start:Button"];
             button->Caption = "Start ...";            
        });
    });

    // The first worker thread gets the job, ten times, every 900 milliseconds, 
    // move the progress bar one step forward
    thread1->call([thread1,thread2]() {
               
        // This is the real job: Move the bar one to the right ten times :-)
        for(int i = 0; i < 10; ++i)
        {
            ::Sleep(900);
            ::PostMessage(hWndPB, PBM_STEPIT, 0, 0);
        }

        // Wenn der Job erledigt ist, stoppen wir vom Haupt-Thread aus beide Worker-Threads
        mainThread->call([thread1,thread2](){
            thread2->stop();
            thread1->stop();
        });
    });
}