
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

		void enqueue(std::string msg);

	protected:
		static void WindowThread(void* data);

	#ifdef win32
		static LRESULT HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);
	#endif

		const int MAX_QUEUE = 10;
		bool is_running;
		std::thread window_thread;

		void* window_handle;
		void* output_handle;

		std::vector<std::string> msgs;
		std::mutex m;
		std::condition_variable cv;

	};
}
