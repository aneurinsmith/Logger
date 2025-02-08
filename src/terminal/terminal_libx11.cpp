
#ifdef libx11
#include "terminal.h"

namespace LOG
{
	Display* dpy;
	Window root;
	int scr;

	Size Terminal::get_size()
	{
		return { 0,0 };
	}

	unsigned int Terminal::get_scroll()
	{
		return 0;
	}



	void Terminal::HandleMessage(XEvent* xe, void* _data)
	{
		Terminal* data = (Terminal*)_data;

		switch (xe->type) {
		case ButtonPress:
			if (xe->xbutton.button == 4) {
				data->on_scroll(1);
			}
			else if (xe->xbutton.button == 5) {
				data->on_scroll(-1);
			}
			return;
		case Expose:
			data->on_draw();
			return;
		case ClientMessage:
			if (xe->xclient.data.l[0] == XInternAtom(dpy, "WM_DELETE_WINDOW", False)) {
				data->on_destroy();
			}
			return;
		}
	}

	void Terminal::init()
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

		XSelectInput(dpy, (Window)handle, ButtonPressMask | ExposureMask);
		XMapWindow(dpy, (Window)handle);
		Atom wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
		XSetWMProtocols(dpy, (Window)handle, &wm_delete_window, 1);
	}

	void Terminal::run()
	{
		XEvent xe;
		while (is_running) {
			XNextEvent(dpy, &xe);
			HandleMessage(&xe, this);
		}
		XCloseDisplay(dpy);
	}

	void Terminal::update()
	{
		if (!XEventsQueued(dpy, QueuedAfterReading)) {
			XEvent ex;
			ex.type = Expose;
			ex.xexpose.window = (Window)handle;
			XSendEvent(dpy, (Window)handle, False, ExposureMask, &ex);
			XFlush(dpy);
		}
	}



	void Terminal::on_scroll(int delta)
	{
		// Get window geometry
		int x, y;
		unsigned int width, height, border_width, depth;
		XGetGeometry(dpy, (Window)handle, &root, &x, &y, &width, &height, &border_width, &depth);

		if (!msgs.empty()) {

			m.lock();
			std::string msg = *(msgs.begin() + msgsPos);
			m.unlock();

			if (delta > 0) {
				if (linePos > 0) {
					linePos--;
				}
				else if (msgsPos > 0) {
					msgsPos--;
					linePos = msg.size() / (width / 8);
				}
			}
			else if (delta < 0) {
				if (linePos < msg.size() / (width / 8)) {
					linePos++;
				}
				else if (msgsPos < msgs.size() - 1) {
					msgsPos++;
					linePos = 0;
				}
			}
		}

		update();
	}

	void Terminal::on_draw()
	{
		// Get window geometry
		int x, y;
		unsigned int width, height, border_width, depth;
		XGetGeometry(dpy, (Window)handle, &root, &x, &y, &width, &height, &border_width, &depth);

		Visual* visual = DefaultVisual(dpy, scr);
		Colormap cmap = DefaultColormap(dpy, scr);
		Pixmap drawMem = XCreatePixmap(dpy, (Window)handle, width, height, depth);
		XftDraw* draw = XftDrawCreate(dpy, drawMem, visual, cmap);
		XftColor color;

		// Set background
		XftColorAllocName(dpy, visual, cmap, "#0C0C0C", &color);
		XftDrawRect(draw, &color, 0, 0, width, height);

		// Create font
		XftColorAllocName(dpy, visual, cmap, "#CCCCCC", &color);
		XftFont* font = XftFontOpenName(dpy, 0, "Consolas:pixelsize=14");



		m.lock();
		if (!msgs.empty()) {

			int y = 1 - linePos;
			for (auto it = msgs.begin() + msgsPos; it != msgs.end() && y <= (int)(height / 16); ++it) {

				std::string msg = *it;
				for (int x = 0; x < msg.size() && y <= (int)(height / 16); x += (width / 8), y++) {
					std::string msg_substr = msg.substr(x, (width / 8));
					if(y > 0) XftDrawStringUtf8(draw, &color, font, 0, (y * 16) - 2, (const FcChar8*)msg_substr.c_str(), msg_substr.size());
				}

			}
		}
		m.unlock();



		// Swap buffers
		XCopyArea(dpy, drawMem, (Window)handle, DefaultGC(dpy, scr), 0, 0, width, height, 0, 0);
		XSetWindowBackgroundPixmap(dpy, (Window)handle, drawMem);

		XftFontClose(dpy, font);
		XFreePixmap(dpy, drawMem);
		XftColorFree(dpy, visual, cmap, &color);
		XftDrawDestroy(draw);
	}

	void Terminal::on_destroy()
	{
		XDestroyWindow(dpy, (Window)handle);
	}
}

#endif
