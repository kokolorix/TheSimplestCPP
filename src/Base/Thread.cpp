
#include "dbg_new.h"
#include<cassert>

#include <map>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <atomic>

#include "Thread.h"
#include <windows.h>
#include <processthreadsapi.h>
#include <assert.h>
#include <codecvt>
#include <locale>
#include "Edit.h"
#include <sstream>
//#include <boost\format.hpp>

const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)
void SetThreadName(DWORD dwThreadID, const char* threadName) {
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable: 6320 6322)
	__try {
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
	}
#pragma warning(pop)
}

using namespace std;
/**
 * @class Thread
 * 
 * @brief This class represents a thread in the system.
 * 
 * A thread can be created and started again, it can be
 * but also existing, running threads can be assigned.
 * 
 * 
 * For newly created threads, the start function can be included, or the 
 * Standard function can be used. The default function #Thread::Impl::standardLoop
 * supports the continuous exchange of function objects via a queue.
 * 
 * 
 * For own start functions or otherwise started threads, like the main thread
 * the processing of the queue must be triggered from outside, via #Thread::processQueue.
 * You can also specify a notification callback to be called,
 * if new function objects have been added to the queue.
 * 

 * All active threads are accessible through the Thread::Manager via a name or the ThreadId
 * 
 * 
 * Using for a background thread is very intuitive:
 * @code {.cpp}
    ThreadPtr thread1 = Thread::Manager["Thread1"];
    if(!thread1->IsRunning)
        thread1->start();

    thread1->call([thread1]() {
		DoTheJob();
	});
 * @endcode
 * Thread1 remains in this case after the job has been completed
 * Stand by and wait for the next job.
 * 
 * If only a specific task is to be completed, this can also be done this way:
 * @code {.cpp}
    ThreadPtr thread1 = Thread::Manager["Thread1"];
    if(!thread1->IsRunning)
        thread1->start([thread1]() {
		DoTheJob();
	});
 * @endcode
 * Thread1 ends in this case with DoTheJob() and will be removed from Manager,
 * if no other ThreadPtr exists in the system 
 * 
 * 
 * Another case is assigning the main thread of an application to a thread object:
 * Globally, the following elements are necessary:
 * @code {.cpp}
ThreadPtr mainThread;           //> The main thread instance 
#define WM_THREAD WM_USER + 1   //> The Windows Message to communicate with the main thread
 * @endcode
 * In the WinMain you have to do
 * @code {.cpp}
    ThreadPtr mainThread = Thread::Manager["MainThread"];
    mainThread->initRunningThread(this_thread::get_id(), [hWnd]() {
         ::PostMessage(hWnd, WM_THREAD, (WPARAM)0, (LPARAM)0); 
         });
 * @endcode
 * And t * he WinProc Function must be extended like this:
 * @code {.cpp}
    case WM_THREAD:
        mainThread->processQueue();
        break;
 * @endcode
 * 
 */

/**
 * @brief The real implementation of Thread.
 * 
 * Hidden from the public eye, here effectively done 
 * 
 */
struct Thread::Impl
{
	Impl(const string &name) 
	: name_(name)
	{}
	string name_;
	ThreadId id_;
	thread thread_;

	function<void()> notify_;

	recursive_mutex mutex_;
	condition_variable_any condition_;
	atomic_bool enqueued_ = false, stopped_ = false;
	queue<function<void()>> queue_;

	void enqueue(function<void()> f);

	void notify();

	static void standardLoop(ThreadPtr pThread);
	void initRunningThread(ThreadId id, function<void()> notify, ThreadPtr pThread);
	void processQueue(unique_lock<recursive_mutex>& lock, size_t maxElements);
};

/**
 * @brief The implementation of the thread manager
 * 
 */
struct Thread::ThreadManager::Impl
{
	static bool isDeleted; ///>An helper construct to prevent the application from crashing on exit.
	~Impl()
	{
		// If the globally stored main thread is released on termination, the manager may already be cleared
		isDeleted = true;
		std::vector<weak_ptr<Thread>> threads;
		for (map<ThreadId, weak_ptr<Thread>>::value_type vt : idMap_)
			threads.push_back(vt.second);

		for ( weak_ptr<Thread> twp : threads)
		{
			ThreadPtr thread = twp.lock(); 
			if (thread)
				thread->stop();
		}
		this_thread::yield();
		for ( weak_ptr<Thread> twp : threads)
		{
			ThreadPtr thread = twp.lock(); 
			if (thread && thread->joinable())
				thread->join();
		}
	}
	recursive_mutex mutex_;
	map<ThreadId, weak_ptr<Thread>> idMap_;
	map<string, weak_ptr<Thread>> nameMap_;

	ThreadPtr getOrCreateThread(const string& name);
	ThreadPtr getThread(const ThreadId &id);
};

/**
 * @copydoc Thread::ThreadManager::operator[](const string &name)
 */
ThreadPtr Thread::ThreadManager::Impl::getOrCreateThread(const string& name)
{
	lock_guard<recursive_mutex> lock(mutex_);
	ThreadPtr threadPtr;
	auto res = nameMap_.insert(make_pair(name, threadPtr));
	if (res.second)
	{
		weak_ptr<Thread>& weakPtr = res.first->second;
		threadPtr = ThreadPtr(new Thread(name));
		weakPtr = threadPtr;
	}
	else
	{
		weak_ptr<Thread>& weakPtr = res.first->second;
		threadPtr = weakPtr.lock();
	}

	return threadPtr;
}

/**
 * @copydoc Thread::ThreadManager::operator[](const ThreadId &id) const
 */
ThreadPtr Thread::ThreadManager::Impl::getThread(const ThreadId &id)
{
	lock_guard<recursive_mutex> lock(mutex_);
	auto it = idMap_.find(id);
	if (it != idMap_.end())
	{
		weak_ptr<Thread>& thread = it->second;
		return thread.lock();
	}
	return ThreadPtr();
}

bool Thread::ThreadManager::Impl::isDeleted = false;
Thread::ThreadManager Thread::Manager;

/**
 * @brief Construct a new Thread:: Thread object
 * 
 * @param name 
 */
Thread::Thread(const string &name)
	: pImpl_(make_unique<Thread::Impl>(name))
	, Name(pImpl_->name_)
	, IsRunning([this]() {
		  return pImpl_->id_ != ThreadId() && !IsStopped;
	  })
	, IsStopped([this]() {
		  return pImpl_->stopped_.load();
	  })
	, Id([this]() {
		  return pImpl_->id_;
	  })
{
}
/**
 * @brief Destroy the Thread:: Thread object
 * 
 */
Thread::~Thread()
{
	if (pImpl_->thread_.joinable())
	{
		if(this_thread::get_id() == pImpl_->thread_.get_id())
		{
			pImpl_->thread_.detach();
		}
		else
		{
			pImpl_->stopped_ = true;
			pImpl_->thread_.join();
		}
	}

	// It is possible that global variables are deleted,  
	// after the manager has already been cleared. 
	// Hence this check here 
	if (!Thread::ThreadManager::Impl::isDeleted)
	{
		lock_guard<recursive_mutex> lock(Thread::Manager.pImpl_->mutex_);
		Thread::Manager.pImpl_->nameMap_.erase(pImpl_->name_);
		Thread::Manager.pImpl_->idMap_.erase(pImpl_->id_);
	}
}

/**
 * @brief 
 * 
 * @param id 
 * @param notify 
 */
void Thread::initRunningThread(ThreadId id, function<void()> notify)
{
	pImpl_->initRunningThread(id, notify, shared_from_this());
}

/**
 * @brief 
 * 
 * @param t 
 */
void Thread::start(thread &&t)
{
	if (pImpl_->thread_.joinable())
	{
		pImpl_->stopped_ = true;
		pImpl_->thread_.join();

		//pImpl_->thread_.detach();
	}

	swap(pImpl_->thread_, t);
	pImpl_->stopped_ = false;
	pImpl_->id_ = pImpl_->thread_.get_id();
	lock_guard<recursive_mutex> lock(Thread::Manager.pImpl_->mutex_);
	Thread::Manager.pImpl_->idMap_.insert(make_pair(pImpl_->id_, shared_from_this()));
}

/**
 * @brief The most normal way to start a new thread.
 */
void Thread::start()
{
	ThreadPtr thisThread = shared_from_this();
	unique_lock<recursive_mutex> lock(pImpl_->mutex_);
	start(thread(Impl::standardLoop, thisThread));
	pImpl_->condition_.wait(lock);
}

/**
 * @brief Stops a running thread as fast as intended.
 * In longer lasting processing it must be provided, at suitable places
 * to cancel
 * @code {.cpp}
    thread2->call([thread2]() {
        while(!thread2->IsStopped) // <--- here we checks every loop the IsStopped Property
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
 * @endcode
 */
void Thread::stop()
{
	pImpl_->stopped_ = true;
	pImpl_->condition_.notify_one();
	this_thread::yield();
	{
		lock_guard<recursive_mutex> lock(Thread::Manager.pImpl_->mutex_);
		Thread::Manager.pImpl_->idMap_.erase(pImpl_->id_);
	}
	pImpl_->id_ = ThreadId();
}

/**
 * @brief 
 * 
 * @param f 
 */
void Thread::enqueue(function<void()> f)
{
	pImpl_->enqueue(f);
}

/**
 * @brief Processes a maximum of the specified number of function objects in the queue
 * 
 * @param maxElements 
 */
void Thread::processQueue(size_t maxElements /*= 10*/)
{
	unique_lock<recursive_mutex> lock(pImpl_->mutex_);
	pImpl_->processQueue(lock, maxElements);
}

/**
 * @brief Checks if the std::thread object identifies an active thread of execution. Specifically, returns true if get_id() != std::thread::id(). So a default constructed thread is not joinable.
 * A thread that has finished executing code, but has not yet been joined is still considered an active thread of execution and is therefore joinable.
 * 
 * @return true if the thread object identifies an active thread of execution, false otherwise 
 */
bool Thread::joinable()
{
	return pImpl_->thread_.joinable();
}

/**
 * @brief Waits for the end of the thread
 * 
 */
void Thread::join()
{
	pImpl_->thread_.join();
}

/**
 * @brief initialize a running thread.
 * 
 * @details 
 * 
 * 
 * @param id 
 * @param notify 
 * @param pThread 
 */
void Thread::Impl::initRunningThread(ThreadId id, function<void()> notify, ThreadPtr pThread)
{
	id_ = id;
	{
		lock_guard<recursive_mutex> lock(Thread::Manager.pImpl_->mutex_);
		Thread::Manager.pImpl_->nameMap_.insert(make_pair(name_, pThread));
	}
	notify_ = notify;
}

/**
 * @brief A new function object is added to the waiting queue
 * 
 * @param f 
 */
void Thread::Impl::enqueue(function<void()> f)
{
	{
		lock_guard<recursive_mutex> lock(mutex_);
		queue_.push(f);
		enqueued_ = true;
	}
	notify();
}

/**
 * @brief The normally used processing loop of a background thread.
 * 
 * @param pThread 
 */
void Thread::Impl::standardLoop(ThreadPtr pThread)
{
	string threadName = pThread->Name;
	//std::wstring threadName = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(tn.c_str());
	//std::wstring threadName(tn.length(), L' ');// = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(tn.c_str());
	//std::copy(tn.begin(), tn.end(), threadName.begin());

	//HRESULT hr = SetThreadDescription(GetCurrentThread(), threadName.c_str());
	//assert(SUCCEEDED(hr));
	SetThreadName(::GetCurrentThreadId(), threadName.c_str());
	
	EditPtr output = Edit::Manager["Output:Edit"];
	output->add((ostringstream() << "start of thread " << threadName << "\r\n").str());
	Thread::Impl* pImpl = pThread->pImpl_.get();
	pImpl->condition_.notify_all();

	do
	{
		unique_lock<recursive_mutex> lock(pThread->pImpl_->mutex_);
		pImpl->condition_.wait(lock, [pImpl] { return pImpl->enqueued_.load() || pImpl->stopped_.load(); });
		pImpl->enqueued_ = false;

		pImpl->processQueue(lock, 10);

	} while (pImpl->stopped_ == false);

	{
		lock_guard<recursive_mutex> lock(Thread::Manager.pImpl_->mutex_);
		Thread::Manager.pImpl_->idMap_.erase(pImpl->id_);
	}
	pImpl->id_ = ThreadId();
	output->add((ostringstream() << "end of thread " << threadName << "\r\n").str());
}
/**
 * @brief process the queue of functors, usually called form notify-functor
 * 
 * @param maxElements ///> if more elements in queue, notify is called
 */
void Thread::Impl::processQueue(unique_lock<recursive_mutex>& lock, size_t maxElements)
{
	for (size_t i = 0; i < maxElements && !queue_.empty(); ++i)
	{
		function<void()> f = queue_.front();
		queue_.pop();
#pragma warning(push)
#pragma warning(disable : 26110)
		lock.unlock();
#pragma warning(pop)
		f();
		lock.lock();

		this_thread::yield();
	}

	if (!queue_.empty())
		notify();
}
/**
 * @brief notify condition or notify-functor
 * 
 */
void Thread::Impl::notify()
{
	if (notify_)
		notify_();
	else
		condition_.notify_one();
}

/**
 * @brief Construct a new Thread:: Thread Manager:: Thread Manager object
 * 
 */
Thread::ThreadManager::ThreadManager()
	: pImpl_(make_unique<Thread::ThreadManager::ThreadManager::Impl>())
{
}

/**
 * @brief Returns the thread with the given name. If none exists yet, it is created now
 * 
 * @param name 
 * @return ThreadPtr 
 */
ThreadPtr Thread::ThreadManager::operator[](const string &name)
{
	return pImpl_->getOrCreateThread(name);
}
/**
 * @brief Returns the Thread object with the resulting ThreadId. If no such thread exists, an empty ThreadPtr is returned. 
 * 
 * @param id 
 * @return ThreadPtr 
 */
ThreadPtr Thread::ThreadManager::operator[](const ThreadId &id) const
{
	return pImpl_->getThread(id);
}

