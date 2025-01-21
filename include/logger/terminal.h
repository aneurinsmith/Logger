
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
			cv.notify_one();
		}

	protected:
		static void WindowThread(void* data);
		static void MessageThread(void* data);

	#ifdef win32
		static LRESULT HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);
	#endif

		const int buffer_size = 10000;
		bool is_running;
		std::thread window_thread;
		std::thread message_thread;

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
