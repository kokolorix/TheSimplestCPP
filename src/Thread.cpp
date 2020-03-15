
#include<cassert>

#include <map>
#include <mutex>
#include <atomic>
#include "Thread.h"

using namespace std;

struct Thread::Impl
{
	Impl(const string &name) 
	: name_(name)
	{}
	string name_;
	ThreadId id_;
	thread thread_;

	function<void()> notify_;

	mutex mutex_;
	condition_variable condition_;
	atomic_bool enqueued_ = false, stopped_ = false;
	queue<function<void()>> queue_;
	void enqueue(function<void()> f);
	static void standardLoop(ThreadPtr pThread);
};

struct Thread::ThreadManager::Impl
{
	mutex mutex_;
	map<ThreadId, ThreadPtr> idMap_;
	map<string, ThreadPtr> nameMap_;
};

Thread::Thread(const string &name)
	: pImpl_(make_unique<Thread::Impl>(name))
	, Name(pImpl_->name_)
	, IsRunning([this]() {
		  return pImpl_->thread_.get_id() != thread::id() || pImpl_->id_ != thread::id();
	  })
	, IsStopped([this]() {
		  return pImpl_->stopped_.load();
	  })
	, Id([this]() {
		  return pImpl_->id_ != thread::id() ? pImpl_->id_ : pImpl_->thread_.get_id();
	  })
{
}

Thread::~Thread()
{
	if (pImpl_->thread_.joinable())
		pImpl_->thread_.join();
}

void Thread::initRunningThread(ThreadId id, function<void()> notify)
{
	pImpl_->id_ = id;
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
}

Thread::ThreadManager Thread::Manager;

void Thread::Impl::enqueue(function<void()> f)
{
	{
		lock_guard<mutex> lock(mutex_);
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
}

void Thread::Impl::standardLoop(ThreadPtr pThread)
{
	string threadName = pThread->Name;
	Thread::Impl* pImpl = pThread->pImpl_.get();
	do
	{
		unique_lock<mutex> lock(pThread->pImpl_->mutex_);
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

	pImpl->thread_.detach();
}

void Thread::processQueue(size_t maxElements /*= 100*/)
{
	unique_lock<mutex> lock(pImpl_->mutex_);
	
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
	lock_guard<mutex> lock(pImpl_->mutex_);
    ThreadPtr &thread = pImpl_->nameMap_[name];
    if (!thread)
        thread = ThreadPtr(new Thread(name));
    return thread;
}

ThreadPtr Thread::ThreadManager::operator[](const ThreadId &id) const
{
	lock_guard<mutex> lock(pImpl_->mutex_);
	auto it = pImpl_->idMap_.find(id);
	if (it == pImpl_->idMap_.end())
		return ThreadPtr();
	return it->second;
}
