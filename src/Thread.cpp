
#include<cassert>

#include <map>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <atomic>

#include "Thread.h"

using namespace std;

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
	static bool isDeleted;
	~Impl()
	{
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


void Thread::start()
{
	start(thread(Impl::standardLoop, shared_from_this()));
}

void Thread::stop()
{
	pImpl_->stopped_ = true;
	pImpl_->condition_.notify_one();
	this_thread::yield();
	Thread::Manager.pImpl_->idMap_.erase(pImpl_->id_);
	pImpl_->id_ = ThreadId();
}

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

void Thread::processQueue(size_t maxElements /*= 100*/)
{
	unique_lock<recursive_mutex> lock(pImpl_->mutex_);
	
	for (size_t i = 0; i < maxElements && !pImpl_->queue_.empty(); ++i)
	{
		function<void()> f = pImpl_->queue_.back();
		pImpl_->queue_.pop();
		lock.unlock();

		f();

		lock.lock();
	}
}

bool Thread::joinable()
{
	return pImpl_->thread_.joinable();
}

void Thread::join()
{
	pImpl_->thread_.join();
}

Thread::ThreadManager::ThreadManager()
	: pImpl_(make_unique<Thread::ThreadManager::ThreadManager::Impl>())
{
}

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

