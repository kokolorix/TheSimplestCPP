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
#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers

#include <windows.h>

#include "dbg_new.h"
#include "utils.hpp"

#include <CommCtrl.h>
#include "Thread.h"
#include "Button.h"
#include "Edit.h"
#include "Progress.h"
#include <fstream>
#include <ctime>
#include <random>
#include "TestCase.h"


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
 * @param nShowCmd          //> The display code , it determines how the application window is initially displayed  
 * @return int WinMain      //> The return value of the application. Traditionally zero if everything was ok
 */
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int WINAPI CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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
    int width = 800, height = 600;
    HWND hWnd = ::CreateWindow(wc.lpszClassName,
                               L"WinApp",
                               WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               (sx - width) / 2,  // center the window on screen
                               (sy - height) / 2,
                               width,
                               height,
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

	 while (GetMessage(&msg, NULL, 0, 0) > 0) {
		 ::TranslateMessage(&msg);
		 ::DispatchMessage(&msg);
    }

    return 0;

}

void OnStartClicked(Button* button);
void OnThreadTestClicked(Button* button);
void OnStartTestClicked(Button* button);
/**
 * @brief 
 * 
 * @param hWnd 
 * @param message 
 * @param wParam 
 * @param lParam 
 * @return LRESULT 
 */
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
		LONG height = rcClient.bottom - rcClient.top;

		ButtonPtr threadTest = Button::Manager["ThreadTest:Button"];
		threadTest->OnClicked = OnThreadTestClicked;
		threadTest->create(hWnd, 140, 20, 100, 30, "Thread-Test");

		ButtonPtr startTest = Button::Manager["StartTest:Button"];
		startTest->OnClicked = OnStartTestClicked;
		startTest->create(hWnd, 260, 20, 100, 30, "Test");

		EditPtr filterTest = Edit::Manager["TestFilter:Edit"];
		filterTest->LeftMargin = 6;
		filterTest->TopMargin = 2;
		filterTest->ClientEdge = true;
		filterTest->create(hWnd, 380, 20, rcClient.right - 400, 30, "*");

		EditPtr output = Edit::Manager["Output:Edit"];
		output->Margins = 6;
		output->ReadOnly = true;
		output->create(hWnd, 20, 60, rcClient.right - 40, (height - (cyVScroll * 2)) - 80);

		ProgressPtr progress1 = Progress::Manager["Progress1"];
		progress1->create(hWnd, 0, height - cyVScroll * 2, rcClient.right, cyVScroll * 2);
		return 0;
	}
	break;

	case WM_COMMAND:
	{
		HWND hCtrl = reinterpret_cast<HWND>(lParam);
		if (HIWORD(wParam) == BN_CLICKED)
		{
			ButtonPtr button = Button::Manager[hCtrl];
			button->execute(BN_CLICKED);
			return 0;
		}
		break;
	}

	case WM_KEYUP:
	{
		if (wParam == VK_ESCAPE)
		{
			::PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		}
	}
	break;

	case WM_LBUTTONDOWN:
		::SetFocus(hWnd);
		return 0;

	case WM_CLOSE:
		::PostQuitMessage(0);
		return 0;

	case WM_THREAD:
		mainThread->processQueue();
		return 0;

	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
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
    
    // Then we get the second worker thread and start it if necessary
    ThreadPtr thread2 = Thread::Manager["Thread2"];

    if (thread1->IsRunning)
    {
		   thread2->stop();
		   thread1->stop();
    }
    else
	 {
		 thread1->start();
		 if (!thread2->IsRunning)
			 thread2->start();
	 }

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
        for(int i = 0; i < 10 && !thread1->IsStopped; ++i)
        {
            ::Sleep(900);
				ProgressPtr progress1 = Progress::Manager["Progress1"];
            progress1->stepIt();
        }
        if(thread1->IsStopped)
		  {
				ProgressPtr progress1 = Progress::Manager["Progress1"];
            progress1->Pos = 0;
		  }

        // if the job is done, we stop the threads from the main-thread
        mainThread->call([thread1,thread2](){
            thread2->stop();
            thread1->stop();
        });
    });
}
/**
 * @brief Thread-Test-Button event handler
 * 
 * @param button 
 */
void OnThreadTestClicked(Button* button)
{
   size_t threadNumber = 9;
	EditPtr output = Edit::Manager["Output:Edit"];
	std::vector<ThreadPtr> threads;
   threads.reserve(threadNumber);
   for (size_t i = 0; i < threadNumber; ++i)
   {
		ostringstream os;
		os << "ThreadTest" << (i + 1);
	   ThreadPtr thread = Thread::Manager[os.str()];
      threads.push_back(thread);
   }

   if(threads.front()->IsRunning)
	{
      for(ThreadPtr thread : threads)
			thread->stop();
      for(ThreadPtr thread : threads)
			thread->join();

		LOG(__FUNCTION__);
		return;
   }
   output->Caption = "";

   for(ThreadPtr thread : threads)
	{
		thread->start();
		LOG(__FUNCTION__);
		thread->call([thread, output]() {
			;
			for (; thread->IsRunning;)
			{
				// Generate a normal distribution around that mean
				std::random_device r;
				// Choose a random mean between 1 and 6
				std::default_random_engine e1(r());
				std::uniform_int_distribution<int> uniform_dist(200, 800);
				int mean = uniform_dist(e1);
				std::seed_seq seed2{ r(), r(), r(), r(), r(), r(), r(), r() };
				std::mt19937 e2(seed2);
				std::normal_distribution<> normal_dist(mean, 2);
				uint32_t wait = static_cast<uint32_t>(std::round(normal_dist(e2)));
				ostringstream os;
				static std::atomic_size_t i = 0;
				os << "Output from " << thread->Name << ": " << ++i << " waiting " << wait << " ms" << endl;
				string line = os.str();
				::Sleep(wait);
				LOG(__FUNCTION__);
				mainThread->call([output, line]() {
					output->add(line);
					});
			}
			});
	}
}

void OnStartTestClicked(Button* button)
{
	EditPtr filterTest = Edit::Manager["TestFilter:Edit"];
	EditPtr output = Edit::Manager["Output:Edit"];
	string pattern = filterTest->Caption;
	output->Caption = "";
	output->add(format("Start tests with filter '%s'\r\n", pattern.c_str()));
	TestCase::runTests(pattern);
}

