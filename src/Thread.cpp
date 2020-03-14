#include <map>
using std::map;

#include "Thread.h"

Thread::Thread(const string &name)
	: name_(name), Name(name_)
	  // , m_id(GenerateId())
	  , Notify(notify_)
	  , Id([this]() {
		   return thread_.get_id() == thread::id() ? this_thread::get_id() : thread_.get_id();
		    })
{
}


Thread::~Thread()
{
	if(thread_.joinable())
		thread_.join();
}


Thread::ThreadManager Thread::Manager;


void Thread::enqueue(function<void()> f)
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


void Thread::standardLoop(ThreadPtr pThread)
{
	string threadName = pThread->Name;
	do
	{
		unique_lock<mutex> lock(pThread->mutex_);
		pThread->condition_.wait(lock, [pThread] { return pThread->enqueued_.load() || pThread->stopped_.load(); });
		pThread->enqueued_ = false;

		while (!pThread->queue_.empty())
		{
			function<void()> f = pThread->queue_.back();
			pThread->queue_.pop();
			lock.unlock();

			f();
			this_thread::yield();

			lock.lock();
		}

	} while (pThread->stopped_ == false);

	pThread->thread_.detach();
}

void Thread::processQueue(size_t maxElements /*= 100*/)
{
	unique_lock<mutex> lock(mutex_);
	
	for (size_t i = 0; i < maxElements && !queue_.empty(); ++i)
	{
		function<void()> f = queue_.back();
		queue_.pop();
		lock.unlock();

		f();

		lock.lock();
	}
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

struct Thread::ThreadManager::Impl
{
	mutex mutex_;
	map<ThreadId, ThreadPtr> idMap_;
	map<string, ThreadPtr> nameMap_;
};

Thread::ThreadManager::ThreadManager() : pImpl_(make_unique<Thread::ThreadManager::ThreadManager::Impl>()){}


ThreadPtr Thread::ThreadManager::operator[](const string& name)
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
