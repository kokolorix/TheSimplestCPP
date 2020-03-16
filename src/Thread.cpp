
#include<cassert>

#include <map>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <atomic>

#include "Thread.h"

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
	static void standardLoop(ThreadPtr pThread);
};

struct Thread::ThreadManager::Impl
{
	static bool isDeleted; ///>An helper construct to prevent the application from crashing on exit.
	~Impl()
	{
		// If the globally stored main thread is released on termination, the manager may already be cleared
		isDeleted = true;
	}
	recursive_mutex mutex_;
	map<ThreadId, weak_ptr<Thread>> idMap_;
	map<string, weak_ptr<Thread>> nameMap_;
};

bool Thread::ThreadManager::Impl::isDeleted = false;
Thread::ThreadManager Thread::Manager;

Thread::Thread(const string &name)
	: pImpl_(make_unique<Thread::Impl>(name))
	, Name(pImpl_->name_)
	, IsRunning([this]() {
		  return pImpl_->id_ != ThreadId();
	  })
	, IsStopped([this]() {
		  return pImpl_->stopped_.load();
	  })
	, Id([this]() {
		  return pImpl_->id_;
	  })
{
}

Thread::~Thread()
{
	if (pImpl_->thread_.joinable())
		pImpl_->thread_.join();

	// It is possible that global variables are deleted,  
	// after the manager has already been cleared. 
	// Hence this check here 
	if (!Thread::ThreadManager::Impl::isDeleted)
	{
		Thread::Manager.pImpl_->nameMap_.erase(pImpl_->name_);
		Thread::Manager.pImpl_->idMap_.erase(pImpl_->id_);
	}
}

void Thread::initRunningThread(ThreadId id, function<void()> notify)
{
	pImpl_->id_ = id;
	Thread::Manager.pImpl_->idMap_.insert(make_pair(pImpl_->id_, shared_from_this()));	
	pImpl_->notify_ = notify;
}

void Thread::enqueue(function<void()> f)
{
	pImpl_->enqueue(f);
}

void Thread::start(thread &&t)
{
	assert(!IsRunning);
	swap(pImpl_->thread_, t);
	pImpl_->stopped_ = false;
	pImpl_->id_ = pImpl_->thread_.get_id();
	Thread::Manager.pImpl_->idMap_.insert(make_pair(pImpl_->id_, shared_from_this()));	
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
	if(notify_)
		notify_();
	else
		condition_.notify_one();
}

/**
 * @brief The most normal way to start a new thread.
 */
void Thread::start()
{
	start(thread(Impl::standardLoop, shared_from_this()));
}

/**
 * @brief Stops a running thread as fast as intended.
 * In länger dauernden Verarbeitungen muss es vorgesehen werden, an geeigneten Stellen
 * abzubrechen:
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
	Thread::Manager.pImpl_->idMap_.erase(pImpl_->id_);
	pImpl_->id_ = ThreadId();
}

/**
 * @brief The normally used processing loop of a background thread.
 * 
 * @param pThread 
 */
void Thread::Impl::standardLoop(ThreadPtr pThread)
{
	string threadName = pThread->Name;
	Thread::Impl* pImpl = pThread->pImpl_.get();
	do
	{
		unique_lock<recursive_mutex> lock(pThread->pImpl_->mutex_);
		pImpl->condition_.wait(lock, [pImpl] { return pImpl->enqueued_.load() || pImpl->stopped_.load(); });
		pImpl->enqueued_ = false;

		while (!pImpl->queue_.empty())
		{
			function<void()> f = pImpl->queue_.back();
			pImpl->queue_.pop();
			lock.unlock();

			f();
			this_thread::yield();

			lock.lock();
		}

	} while (pImpl->stopped_ == false);

	Thread::Manager.pImpl_->idMap_.erase(pImpl->id_);
	pImpl->id_ = ThreadId();
	pImpl->thread_.detach();
}

/**
 * @brief Processes a maximum of the specified number of function objects in the queue
 * 
 * @param maxElements 
 */
void Thread::processQueue(size_t maxElements /*= 10*/)
{
	unique_lock<recursive_mutex> lock(pImpl_->mutex_);
	
	for (size_t i = 0; i < maxElements && !pImpl_->queue_.empty(); ++i)
	{
		function<void()> f = pImpl_->queue_.back();
		pImpl_->queue_.pop();
		lock.unlock();

		f();
		this_thread::yield();

		lock.lock();
	}
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
	lock_guard<recursive_mutex> lock(pImpl_->mutex_);
	ThreadPtr threadPtr;
	auto res = pImpl_->nameMap_.insert(make_pair(name , threadPtr));
	if(res.second)
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
 * @brief Returns the Thread object with the resulting ThreadId. If no such thread exists, an empty ThreadPtr is returned. 
 * 
 * @param id 
 * @return ThreadPtr 
 */
ThreadPtr Thread::ThreadManager::operator[](const ThreadId &id) const
{
	lock_guard<recursive_mutex> lock(pImpl_->mutex_);
	auto it = pImpl_->idMap_.find(id);
	if (it != pImpl_->idMap_.end())
	{
		weak_ptr<Thread> &thread = it->second;
		return thread.lock();
	}
	return ThreadPtr();
}

