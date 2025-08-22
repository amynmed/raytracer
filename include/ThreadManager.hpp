#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>


class ThreadManager
{
public:
	ThreadManager() = default;

	template <typename Ret, typename... Args>
	void process(const std::function<Ret(Args...)>& callback, Args... args)
	{
		callback(args...);
	}

	void acquire()
	{
		std::unique_lock<std::mutex> lock(mutex);
		cond.wait(lock, [this] { return m_active_threads < m_max_threads; });
		++m_active_threads;
	}

	void release()
	{
		std::unique_lock<std::mutex> lock(mutex);
		--m_active_threads;
		cond.notify_one();
	}

	int max_threads() { return m_max_threads; }

	//
private:
	int                     m_active_threads = 0;
	int                     m_max_threads    = std::thread::hardware_concurrency();
	std::mutex              mutex;
	std::condition_variable cond;
};