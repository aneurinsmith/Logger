
#ifdef libx11
#include "terminal.h"

#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xft/Xft.h>

namespace LOG
{
	Display* dpy;
	Window root;
	int scr;
	GC gc;

	void Terminal::HandleMessage(XEvent* xe, void* _data)
	{
		Terminal* data = (Terminal*)_data;

		switch (xe->type) {
		case ClientMessage:
			if (xe->xclient.data.l[0] == XInternAtom(dpy, "WM_DELETE_WINDOW", False)) {
				data->on_destroy();
			}
			return;
		case Expose:
			data->on_draw();
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
			0, BlackPixel(dpy, scr), WhitePixel(dpy, scr));
		if (!handle) {
			throw std::runtime_error("Could not create the window");
		}
		gc = XCreateGC(dpy, (Window)handle, 0, NULL);

		XSelectInput(dpy, (Window)handle, ExposureMask);
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
		}
	}



	void Terminal::on_draw()
	{
		// Get window geometry
		int x, y;
		unsigned int width, height, border_width, depth;
		XGetGeometry(dpy, (Window)handle, &root, &x, &y, &width, &height, &border_width, &depth);

		Visual* visual = DefaultVisual(dpy, scr);
		Colormap cmap = DefaultColormap(dpy, scr);
		Pixmap drawMem = XCreatePixmap(dpy, (Window)handle,
			width,
			height,
			depth);
		XftDraw* draw = XftDrawCreate(dpy, drawMem, visual, cmap);
		XftColor color;

		// Set background
		XftColorAllocName(dpy, visual, cmap, "#0C0C0C", &color);
		XftDrawRect(draw, &color, 0, 0, width, height);

		// Create font
		XftColorAllocName(dpy, visual, cmap, "#CCCCCC", &color);
		XftFont* font = XftFontOpenName(dpy, 0, "Consolas:size=11");



		if (!msgs.empty()) {
			for (int i = 0, j = 1; i < msgs.back().size() && j < (height/14); i += (width/9), j++) {
				std::string msg = msgs.back().substr(i, width/9);
				XftDrawStringUtf8(draw, &color, font, 0, j*14, 
					(const FcChar8*)msg.c_str(), msg.size());
			}
		}


		// Swap buffers
		XCopyArea(dpy, drawMem, (Window)handle, gc, 0, 0, width, height, 0, 0);
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
