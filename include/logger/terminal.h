
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
#elif libx11
#include <X11/Xlib.h>
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
		static LRESULT CALLBACK HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);
	#elif libx11
		void HandleMessage(Display* dpy, Window wnd, XEvent xe);
	#endif

		const int MAX_QUEUE = 100;
		bool is_running;
		std::thread thread;

		void* handle;

		std::vector<std::string> msgs;
		std::mutex m;
		std::condition_variable cv;

	};
}
