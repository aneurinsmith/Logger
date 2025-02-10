
#ifdef libx11
#include "console.h"

namespace LOG
{
	Display* dpy;
	Window root;
	int scr;

	bool is_dragging = false;

	unsigned int Console::get_width()
	{
		int x, y;
		unsigned int width, height, border_width, depth;
		XGetGeometry(dpy, (Window)handle, &root, &x, &y, &width, &height, &border_width, &depth);
		return width;
	}

	unsigned int Console::get_height()
	{
		int x, y;
		unsigned int width, height, border_width, depth;
		XGetGeometry(dpy, (Window)handle, &root, &x, &y, &width, &height, &border_width, &depth);
		return height;
	}

	void Console::HandleMessage(XEvent* xe, void* _data)
	{
		Console* data = (Console*)_data;

		switch (xe->type) {
		case Expose:
			data->on_draw();
			break;
		case ButtonPress:
			if (xe->xbutton.x > data->get_width() - 20) {
				is_dragging = true;
			}
			if (xe->xbutton.button == 4) {
				data->scroll_up();
			}
			else if (xe->xbutton.button == 5) {
				data->scroll_down();
			}
			data->on_draw();
			break;
		case ButtonRelease:
			is_dragging = false;
			break;

		case MotionNotify: {
			if (is_dragging) {
				int y = xe->xmotion.y;
				if (y < 0) {
					data->set_scrollPos(0);
				}
				else if (y > data->get_height()) {
					data->set_scrollPos(100);
				}
				else {
					data->set_scrollPos(((float)y / data->get_height()) * 100);
				}
			}
			data->on_draw();
			break;
		}
		case ConfigureNotify:
			data->on_size();
			break;

		case ClientMessage:
			if (xe->xclient.data.l[0] == XInternAtom(dpy, "WM_DELETE_WINDOW", False)) {
				data->on_destroy();
			}
			break;
		}
	}

	void Console::init()
	{
		XInitThreads();
		XftInit(NULL);

		dpy = XOpenDisplay(NULL);
		if (!dpy) {
			throw std::runtime_error("Could not find default display");
		}

		scr = DefaultScreen(dpy);
		root = DefaultRootWindow(dpy);
		if (!root) {
			throw std::runtime_error("Could not find root window");
		}

		handle = (void*)XCreateSimpleWindow(
			dpy, root,
			0, 0,
			600, 400,
			0, 0, 0);
		if (!handle) {
			throw std::runtime_error("Could not create the window");
		}

		XSelectInput(dpy, (Window)handle, ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | ExposureMask | StructureNotifyMask);
		XMapWindow(dpy, (Window)handle);
		Atom wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
		XSetWMProtocols(dpy, (Window)handle, &wm_delete_window, 1);
	}

	void Console::run()
	{
		XEvent xe;
		while (is_running) {
			XNextEvent(dpy, &xe);
			HandleMessage(&xe, this);
		}
		XCloseDisplay(dpy);
	}

	void Console::update()
	{
		if (!XEventsQueued(dpy, QueuedAfterReading)) {
			XEvent ex;
			ex.type = Expose;
			ex.xexpose.window = (Window)handle;
			XSendEvent(dpy, (Window)handle, False, ExposureMask, &ex);
			XFlush(dpy);
		}
	}



	void Console::on_draw()
	{
		Visual* visual = DefaultVisual(dpy, scr);
		Colormap cmap = DefaultColormap(dpy, scr);
		Pixmap drawMem = XCreatePixmap(dpy, (Window)handle, get_width(), get_height(), 24);
		XftDraw* draw = XftDrawCreate(dpy, drawMem, visual, cmap);
		XftColor color;

		// Set background
		XftColorAllocName(dpy, visual, cmap, "#0C0C0C", &color);
		XftDrawRect(draw, &color, 0, 0, get_width(), get_height());

		// Create font
		XftColorAllocName(dpy, visual, cmap, "#CCCCCC", &color);
		XftFont* font = XftFontOpenName(dpy, 0, "Consolas:pixelsize=14");

		// Render messages
		m.lock();
		if (!msgs.empty()) {

			int y = 1 - linePos;
			for (auto it = msgs.begin() + msgsPos; it != msgs.end() && y <= (int)(get_height()/16); ++it) {

				std::string msg = *it;
				for (int x = 0; x < msg.size() && y <= (int)(get_height()/16); x += ((get_width()-16)/8), y++) {
					std::string msg_substr = msg.substr(x, ((get_width()-16)/8));
					if (y > 0) XftDrawStringUtf8(draw, &color, font, 0, (y * 16) - 2, (const FcChar8*)msg_substr.c_str(), msg_substr.size());
				}

			}
		}
		m.unlock();

		// Draw scrollbar
		XftColorAllocName(dpy, visual, cmap, "#171717", &color);
		XftDrawRect(draw, &color, 
			get_width()-16, 0, 
			16, get_height());

		XftColorAllocName(dpy, visual, cmap, "#4D4D4D", &color);
		XftDrawRect(draw, &color, 
			get_width()-16, (get_height()-20) * (float)get_scrollPos()/100, 
			16, 20);

		// Swap buffers
		XCopyArea(dpy, drawMem, (Window)handle, DefaultGC(dpy, scr), 0, 0, get_width(), get_height(), 0, 0);
		XSetWindowBackgroundPixmap(dpy, (Window)handle, drawMem);

		XftFontClose(dpy, font);
		XFreePixmap(dpy, drawMem);
		XftColorFree(dpy, visual, cmap, &color);
		XftDrawDestroy(draw);
	}

	void Console::on_destroy()
	{
		XDestroyWindow(dpy, (Window)handle);
	}
}

#endif
