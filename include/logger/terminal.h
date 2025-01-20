
#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>

#ifdef win32
#include <windows.h>
#undef ERROR
#endif

namespace LOG
{
	class Terminal
	{
	public:

		Terminal();
		~Terminal();

		void enqueue(std::string msg)
		{
			q.push(msg);
		}

	protected:
		static void ThreadEntry(void* data);

	#ifdef win32
		static LRESULT CALLBACK HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);
	#endif

		bool is_running;
		std::thread thread_handle;
		void* window_handle;
		void* output_handle;
		std::queue<std::string> q;
		std::mutex m;
		std::condition_variable cv;

		std::string dequeue()
		{
			std::string _msg = q.front();
			q.pop();

			return _msg;
		}

	};
}
