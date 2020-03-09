#include <map>
using std::map;

#include "Thread.h"

Thread::Thread(const string& name)
	:   name_(name)
	, Name(name_)
	// , m_id(GenerateId())
	, Id([this](){return thread_.get_id() ;})
{
}


Thread::~Thread()
{
	if(thread_.joinable())
		thread_.join();
}


Thread::ThreadPool Thread::Pool;


void Thread::enqueue(function<void()> f)
{
	{
		lock_guard<mutex> lock(mutex_);
		queue_.push(f);
		enqueued_ = true;
	}
	condition_.notify_one();
}


void Thread::standardLoop(ThreadPtr pThread)
{
	string threadName = pThread->Name;
	do
	{
		unique_lock<mutex> lock(pThread->mutex_);
		pThread->condition_.wait(lock, [pThread] { return pThread->enqueued_.load(); });
		pThread->enqueued_ = false;

		while (pThread->queue_.empty() == false)
		{
			function<void()> f = pThread->queue_.back();
			pThread->queue_.pop();
			lock.unlock();

			f();
			this_thread::yield();

			lock.lock();
		}

	} while (pThread->stopped_ == false);

}

void Thread::start()
{
	thread_ = thread(standardLoop, shared_from_this());
}

void Thread::stop()
{
	stopped_ = true;
	condition_.notify_one();
	this_thread::yield();
}

struct Thread::ThreadPool::Impl
{
	mutex mutex_;
	map<ThreadId, ThreadPtr> threadMap_;
	map<string, ThreadId> threadIdMap_;
};

Thread::ThreadPool::ThreadPool() : pImpl_(make_unique<Thread::ThreadPool::ThreadPool::Impl>()){}


ThreadPtr Thread::ThreadPool::operator[](const string& name)
{
	lock_guard<mutex> lock(pImpl_->mutex_);
	ThreadId& id = pImpl_->threadIdMap_[name];
	auto it = pImpl_->threadMap_.find(id);
	if(it == pImpl_->threadMap_.end())
	{
		ThreadPtr pThread(new Thread(name));
		it = pImpl_->threadMap_.insert(make_pair(pThread->Id, pThread)).first;
	}
	return it->second;
}

ThreadPtr Thread::ThreadPool::operator[](const ThreadId &id) const
{
	lock_guard<mutex> lock(pImpl_->mutex_);
	auto it = pImpl_->threadMap_.find(id);
	if (it == pImpl_->threadMap_.end())
		return ThreadPtr();
	return it->second;
}
