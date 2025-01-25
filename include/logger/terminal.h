
#pragma once
#include <gtk/gtk.h>

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>

namespace LOG
{
	class Terminal
	{
	public:

		Terminal()
		{
			std::unique_lock<std::mutex> lk(m);
			thread = std::thread(&Terminal::ThreadStart, this);
			//cv.wait(lk);
		}
		~Terminal()
		{
			gtk_main_quit();
			thread.join();
		}

		void push(std::string msg)
		{
			//m.lock();
			if (msgs.size() >= MAX_QUEUE) {
				msgs.erase(msgs.begin());
			}
			msgs.push_back(msg);
			if (msgsPos < msgs.size() - 1) msgsPos++;
			//m.unlock();
		}

	protected:
		static void ThreadStart(void* _data)
		{
			Terminal* data = (Terminal*)_data;
			GtkApplication* app = gtk_application_new("org.gtk.terminal", G_APPLICATION_FLAGS_NONE);
			g_signal_connect(app, "activate", G_CALLBACK(on_activate), data);
			g_application_run(G_APPLICATION(app), 0, 0);
		}
		static void on_activate(GtkApplication* app, gpointer _data);
		static void on_realise(GtkWidget* wnd, gpointer _data);
		static gboolean on_draw(GtkWidget* wnd, cairo_t* cr, gpointer _data);

	private:
		const int MAX_QUEUE = 100;
		int msgsPos = 0;
		int linePos = 0;

		GtkWidget* window;
		GtkWidget* drawable;

		bool is_running;
		std::thread thread;

		std::vector<std::string> msgs;
		std::condition_variable cv;
		std::mutex m;

	};
}
