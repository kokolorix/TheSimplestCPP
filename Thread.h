#pragma once

#include <string>
using std::string;

#include <queue>
using std::queue;

#include <memory>
using std::const_pointer_cast;
using std::dynamic_pointer_cast;
using std::enable_shared_from_this;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::static_pointer_cast;
using std::unique_ptr;

#include <thread>
using std::thread;
namespace this_thread = std::this_thread;

#include <mutex>
using std::lock_guard;
using std::mutex;
using std::condition_variable;
using std::unique_lock;

#include <atomic>
using std::atomic_bool;

#include <functional>
using std::function;

#include "Property.hpp"

class Thread;
using ThreadPtr = shared_ptr<Thread> ;
using ThreadId = thread::id;

class Thread : public enable_shared_from_this<Thread>
{
	explicit Thread(const string &name);
	friend inline shared_ptr<Thread> make_shared();

public:
	virtual ~Thread();
	static struct ThreadPool
	{
		ThreadPool();
		virtual ~ThreadPool() = default;
		ThreadPtr operator[](const string &name);
		ThreadPtr operator[](const ThreadId &id) const;

	private:
		struct Impl;
		unique_ptr<Impl> pImpl_;
	} Pool;

public:
	template <class _Fn, class... _Args>
	void start(_Fn &&_Fx, _Args &&... _Ax)
	{
		thread_ = thread(_Fx, forward<_Args>(_Ax)...);
	}

	template <class _Fn, class... _Args>
	void call(_Fn &&_Fx, _Args &&... _Ax)
	{
		auto f = bind(_Fx, forward<_Args>(_Ax)...);
		enqueue(f);
	}

	void start();
	void stop();
	static void standardLoop(ThreadPtr pThread);

	bool joinable() { return thread_.joinable(); }
	void join() { thread_.join(); }
	thread::id get_id() { return thread_.get_id(); }

private:
	string name_;
	// ThreadId m_id;
	thread thread_;

	mutex mutex_;
	condition_variable condition_;
	atomic_bool enqueued_ = false, stopped_ = false;
	queue<function<void()>> queue_;
	void enqueue(function<void()> f);

public:
	PropertyR<string> Name;
	PropertyR<ThreadId> const Id;
};
