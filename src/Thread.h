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

#include <functional>
using std::function;

#include "Property.hpp"

class Thread;
using ThreadPtr = shared_ptr<Thread> ;
using ThreadId = thread::id;

class Thread : public enable_shared_from_this<Thread>
{
	explicit Thread(const string &name);
	// friend inline shared_ptr<Thread> make_shared<Thread>(const string &);

public:
	virtual ~Thread();
	static struct ThreadManager
	{
		ThreadManager();
		virtual ~ThreadManager() = default;
		ThreadPtr operator[](const string &name);
		ThreadPtr operator[](const ThreadId &id) const;

	private:
		struct Impl;
		unique_ptr<Impl> pImpl_;
	} Manager;

public:
	template <class _Fn, class... _Args>
	void start(_Fn &&_Fx, _Args &&... _Ax)
	{
		start(thread(_Fx, forward<_Args>(_Ax)...));
	}

	template <class _Fn, class... _Args>
	void call(_Fn &&_Fx, _Args &&... _Ax)
	{
		auto f = bind(_Fx, forward<_Args>(_Ax)...);
		enqueue(f);
	}

	void start();
	void stop();

	void initRunningThread(ThreadId id, function<void()> notify);
	void processQueue(size_t maxElements = 100);

	bool joinable();
	void join();

private:
	struct Impl;
	unique_ptr<Impl> pImpl_;
	void enqueue(function<void()> f);
	void start(thread&& t);
	
public:
	PropertyR<bool> const IsRunning;
	PropertyR<bool> const IsStopped;
	PropertyR<ThreadId> const Id;
	PropertyR<string> Name;
};
