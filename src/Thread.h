#pragma once

#include <string>
using std::string;

#include <memory>
using std::enable_shared_from_this;
using std::make_unique;
using std::shared_ptr;
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
		friend class Thread;
	} Manager;

public:
	template <class _Fn, class... _Args>
	void start(_Fn &&_Fx, _Args &&... _Ax)
	{
		ThreadPtr thisThread = shared_from_this();
		start(thread([thisThread, _Fn](typename std::forward<_Args>(_Ax)...){
			_Fn(std::forward<_Args>(_Ax)...);
		}));
	}

	template <class _Fn, class... _Args>
	void call(_Fn &&_Fx, _Args &&... _Ax)
	{
		auto f = bind(_Fx, std::forward<_Args>(_Ax)...);
		enqueue(f);
	}

	void start();
	void stop();

	void initRunningThread(ThreadId id, function<void()> notify);
	void processQueue(size_t maxElements = 10);

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
