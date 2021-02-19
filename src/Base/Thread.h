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

extern ThreadPtr mainThread;

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
		ThreadPtr find(const string& name) const;
		ThreadPtr currentThread() const;

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
		start_(thread([thisThread, _Fn](typename std::forward<_Args>(_Ax)...){
			_Fn(std::forward<_Args>(_Ax)...);
		}));
	}

	template <class _Fn, class... _Args>
	void enqueue(_Fn &&_Fx, _Args &&... _Ax)
	{
		auto f = bind(_Fx, std::forward<_Args>(_Ax)...);
		enqueue_(f);
	}

	void start();
	void stop();

	static void sleep(size_t milliseconds);

	bool joinable();
	void join();

	void initRunningThread(ThreadId id, function<void()> notify);
	void processQueue(size_t maxElements = 10);

private:
	struct Impl;
	unique_ptr<Impl> pImpl_;
	void enqueue_(function<void()> f);
	void start_(thread&& t);
	
public:
	PropertyR<bool> IsRunning;
	PropertyR<bool> IsStopped;
	PropertyR<ThreadId> Id;
	PropertyR<string> Name;
};
