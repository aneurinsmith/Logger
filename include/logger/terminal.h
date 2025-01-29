
#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>

#ifdef win32
#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0605
#include <windows.h>
#include <WinUser.h>
#undef ERROR
#elif libx11
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xft/Xft.h>
#endif

namespace LOG
{
	class Terminal
	{
	public:

		Terminal()
		{
			std::unique_lock<std::mutex> lk(m);
			thread = std::thread(&Terminal::ThreadStart, this);
			cv.wait(lk);
		}

		~Terminal()
		{
			is_running = false;
			thread.join();
		}

		void push(std::string msg)
		{
			m.lock();
			if (msgs.size() >= MAX_QUEUE) {
				msgs.erase(msgs.begin());
			}
			msgs.push_back(msg);
			if (msgsPos < msgs.size() - 1) msgsPos++;
			m.unlock();
			update();
		}

	private:

	#ifdef win32
		static LRESULT CALLBACK HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);
	#elif libx11
		static void HandleMessage(XEvent* xe, void* _data);
	#endif

		static void ThreadStart(void* _data)
		{
			Terminal* data = (Terminal*)_data;
			data->init();
			data->cv.notify_all();
			data->run();
		}

		void init();
		void run();
		void update();

		void on_scroll(int delta);
		void on_draw();
		void on_destroy();

		const unsigned int MAX_QUEUE = 20;
		const char* WINDOW_NAME = "Logger";

		int msgsPos = 0;
		int linePos = 0;

		void* handle;

		bool is_running = true;
		std::thread thread;

		std::vector<std::string> msgs;
		std::condition_variable cv;
		std::mutex m;

	};
}
