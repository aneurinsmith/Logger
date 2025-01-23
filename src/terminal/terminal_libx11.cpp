
#ifdef libx11
#include "terminal.h"

#include <X11/Xlib.h>

namespace LOG
{
	Terminal::Terminal() :
		is_running(true)
	{
		std::unique_lock<std::mutex> lk(m);
		thread = std::thread(&Terminal::WindowThread, this);
		cv.wait(lk);
	}

	Terminal::~Terminal()
	{
		is_running = false;
		thread.join();
	}

	void Terminal::enqueue(std::string msg)
	{

	}

	void Terminal::WindowThread(void* _data)
	{
		Terminal* data = (Terminal*)_data;

		Display* display = XOpenDisplay(0);
		if (!display) {
			throw std::runtime_error("Could not find default display");
		}

		Window root = DefaultRootWindow(display);
		if (!root) {
			throw std::runtime_error("Could not find root window");
		}

		data->handle = (void*)XCreateSimpleWindow(
			display, root, 
			0, 0, 
			600, 400, 
			0, 0, 0xffffff);
		if (!data->handle) {
			throw std::runtime_error("Could not create the window");
		}

		data->cv.notify_all();

		XMapWindow(display, (Window)data->handle);
		Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
		XSetWMProtocols(display, (Window)data->handle, &wm_delete_window, 1);

		XEvent msg;
		while (data->is_running) {
			XNextEvent(display, &msg);

			switch (msg.type) {
				case ClientMessage: {
					if (msg.xclient.data.l[0] == wm_delete_window) {
						XDestroyWindow(display, (Window)data->handle);
						data->is_running = false;
					}
				}
			}
		}

		XCloseDisplay(display);
	}
}

#endif
