
#pragma once
#include "base_sink.h"
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <cmath>
#include <iostream>
#include <atomic>

#ifdef win32
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#define WINVER 0x0605
	#include <Windows.h>
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
	class Console
	{
	public:

		Console(std::string name = "", int size = 100)
		{
			std::stringstream stream;
			stream << "Log Viewer";
			if (!name.empty()) {
				stream << " [" << name.c_str() << "]";
			}

			WINDOW_NAME = stream.str();
			MAX_QUEUE = size;

			std::unique_lock<std::mutex> lk(m);
			thread = std::thread(&Console::ThreadStart, this);
			cv.wait(lk);
		}

		~Console()
		{
			is_running = false;
			thread.join();
		}

		void push(Message msg)
		{
			m.lock();
			msgs.push_back(msg);
			if (msgs.size() > MAX_QUEUE) {
				msgs.erase(msgs.begin());
			}
			else if ((msgsPos < msgs.size() - 1)) {
				msgsPos++;
			}
			m.unlock();
			update();
		}

	private:

		// Window methods and variables
		void* handle;
		unsigned int MAX_QUEUE;
		std::string WINDOW_NAME;

		std::atomic<bool> isUpdateScheduled;
	#ifdef libx11
		Display* dpy;
		Window root;
		int scr;
		bool is_dragging = false;
	#endif

		static void ThreadStart(void* _data)
		{
			Console* data = (Console*)_data;
			data->init();
			data->cv.notify_all();
			data->run();
		}

	#ifdef win32
		static LRESULT CALLBACK HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);
	#elif libx11
		static void HandleMessage(XEvent* xe, void* _data);
	#endif

		void init();
		void run();
		void update();

		void on_size()
		{
			m.lock();
			if (!msgs.empty()) {
				std::string topMsg = get_topMsg();
				int topMsgHeight = (topMsg.size() - 1) / (get_width() / 8);

				if (msgsPos == msgs.size()-1) {
					if (linePos > topMsgHeight) {
						linePos--;
					}
				}
			}
			m.unlock();
		}
		void on_draw();
		void on_destroy();

		unsigned int get_width();
		unsigned int get_height();



		std::string get_topMsg()
		{
			Message m = *(msgs.begin() + msgsPos);
			return m.get_fullString();
		}



		// Scollbar methods and variables
		int msgsPos = 0;
		int linePos = 0;

		void scroll_up()
		{
			m.lock();
			if (!msgs.empty()) {
				if (linePos > 0) {
					linePos--;
				}
				else if (msgsPos > 0) {
					msgsPos--;
					std::string topMsg = get_topMsg();
					linePos = (topMsg.size() - 1) / (get_width()/8);
				}
			}
			m.unlock();
		}

		void scroll_down()
		{
			m.lock();
			if (!msgs.empty()) {
				std::string topMsg = get_topMsg();
				if (linePos < (topMsg.size() - 1) / (get_width()/8)) {
					linePos++;
				}
				else if (msgsPos < msgs.size() - 1) {
					msgsPos++;
					linePos = 0;
				}
			}
			m.unlock();
		}

		void set_scrollPos(unsigned int scrollPos)
		{
			m.lock();
			if (!msgs.empty()) {
				std::string topMsg = get_topMsg();
				int topMsgHeight = (topMsg.size() - 1) / (get_width()/8);
				float adjustedPos = (float)(MAX_QUEUE - 0.01) * ((float)scrollPos / 10000);

				if (adjustedPos < msgs.size()) {
					msgsPos = (int)adjustedPos;
					linePos = std::round(topMsgHeight * (adjustedPos - (int)adjustedPos));
				}
			}
			m.unlock();
		}

		unsigned int get_scrollPos()
		{
			unsigned int scrollPos = 0;

			m.lock();
			if (!msgs.empty()) {
				std::string topMsg = get_topMsg();
				int topMsgHeight = ((topMsg.size() - 1) / (get_width()/8)) + 1;
				scrollPos = ((float)(linePos+(msgsPos * topMsgHeight)) 
							/ (((MAX_QUEUE - 1) * topMsgHeight) + (topMsgHeight - 1)))
							* 10000;
			}
			m.unlock();

			return scrollPos;
		}



		// Thread and shared resource variables
		bool is_running = true;
		std::thread thread;
		
		std::vector<Message> msgs;
		std::condition_variable cv;
		std::mutex m;
	};
}
