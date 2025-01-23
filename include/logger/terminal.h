
#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>
#include <sstream>

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
			std::lock_guard<std::mutex> lk(m);
			q.push(msg);

			if (msgs.size() >= MAX_QUEUE) {
				msgs.erase(msgs.begin());
			}
			msgs.push_back(msg);

			cv.notify_one();
			RedrawWindow((HWND)window_handle, 0, 0, RDW_INVALIDATE);
		}

	protected:
		static void WindowThread(void* data);
		static void MessageThread(void* data);

	#ifdef win32
		static LRESULT HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);
	#endif

		const int MAX_QUEUE = 30;
		char* buffer;
		bool is_running;
		std::thread window_thread;
		std::thread message_thread;

		void* window_handle;
		void* output_handle;

		std::vector<std::string> msgs;
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
