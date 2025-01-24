
#ifdef libx11
#include "terminal.h"

#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>
#include <stdio.h>

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

		Display* dpy = XOpenDisplay(0);
		if (!dpy) {
			throw std::runtime_error("Could not find default display");
		}

		Window root = DefaultRootWindow(dpy);
		if (!root) {
			throw std::runtime_error("Could not find root window");
		}

		data->handle = (void*)XCreateSimpleWindow(
			dpy, root,
			0, 0, 
			600, 400, 
			0, 0, 0x0C0C0C);
		if (!data->handle) {
			throw std::runtime_error("Could not create the window");
		}

		data->cv.notify_all();

		XSelectInput(dpy, (Window)data->handle, ExposureMask);
		XMapWindow(dpy, (Window)data->handle);
		Atom wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
		XSetWMProtocols(dpy, (Window)data->handle, &wm_delete_window, 1);

		XEvent xe;
		while (data->is_running) {

			if (XPending(dpy)) {
				XNextEvent(dpy, &xe);
				data->HandleMessage(dpy, (Window)data->handle, xe);
			}

			switch (xe.type) {
				
			}
		}

		XCloseDisplay(dpy);
	}



	void Terminal::HandleMessage(Display* dpy, Window wnd, XEvent xe)
	{
		switch (xe.type) {
			case ClientMessage: {
				if (xe.xclient.data.l[0] == XInternAtom(dpy, "WM_DELETE_WINDOW", False)) {
					XDestroyWindow(dpy, wnd);
					is_running = false;
				}
				break;
			}
			case Expose: {

				int screen = DefaultScreen(dpy);
				Visual* visual = DefaultVisual(dpy, screen);
				Colormap cmap = DefaultColormap(dpy, screen);
				Pixmap drawMem = XCreatePixmap(dpy, wnd,
					DisplayWidth(dpy, screen), 
					DisplayHeight(dpy, screen), 
					DefaultDepth(dpy, screen));
				XftDraw* draw = XftDrawCreate(dpy, drawMem, visual, cmap);

				// Set background
				XftColor color;
				XftColorAllocName(dpy, visual, cmap, "#0C0C0C", &color);
				XftDrawRect(draw, &color, 0, 0, DisplayWidth(dpy, screen), DisplayHeight(dpy, screen));

				// Create font
				XftFont* font = XftFontOpenName(dpy, 0, "Consolas:size=11");
				XftColorAllocName(dpy, visual, cmap, "#CCCCCC", &color);



				XftDrawStringUtf8(draw, &color, font, 0, 50, (const FcChar8*)"Term", 4);



				// Swap buffers
				XCopyArea(dpy, drawMem, wnd, DefaultGC(dpy, screen), 0, 0,
					DisplayWidth(dpy, screen),
					DisplayHeight(dpy, screen), 
					0, 0);

				XftColorFree(dpy, visual, cmap, &color);
				XftDrawDestroy(draw);

				break;
			}
		}
	}
}

#endif
