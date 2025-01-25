
#ifdef libx11
#include "terminal.h"

#include <wayland-client.h>
#include <libdecor-0/libdecor.h>

namespace LOG
{
	static Display* dpy;
	static Window root;

	void Terminal::HandleMessage(XEvent* xe, void* _data)
	{
		Terminal* data = (Terminal*)_data;

		switch (xe->type) {
		case ClientMessage:
			if (xe->xclient.data.l[0] == XInternAtom(dpy, "WM_DELETE_WINDOW", False)) {
				data->on_destroy();
			}
			break;
		case Expose:
			data->on_draw();
			break;
		}
	}

	void Terminal::init()
	{
		dpy = XOpenDisplay(0);
		if (!dpy) {
			throw std::runtime_error("Could not find default display");
		}

		root = DefaultRootWindow(dpy);
		if (!root) {
			throw std::runtime_error("Could not find root window");
		}

		handle = (void*)XCreateSimpleWindow(
			dpy, root,
			0, 0,
			600, 400,
			0, 0, 0x0C0C0C);
		if (!handle) {
			throw std::runtime_error("Could not create the window");
		}

		XSetWindowAttributes attrs;
		attrs.override_redirect = True;
		XChangeWindowAttributes(dpy, (Window)handle, CWOverrideRedirect, &attrs);

		XSelectInput(dpy, (Window)handle, ExposureMask);
		XMapWindow(dpy, (Window)handle);

		Atom wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
		XSetWMProtocols(dpy, (Window)handle, &wm_delete_window, 1);
	}

	void Terminal::run()
	{
		XEvent xe;
		while (is_running) {
			if (XPending(dpy)) {
				XNextEvent(dpy, &xe);
				HandleMessage(&xe, this);
			}
		}
		XCloseDisplay(dpy);
	}



	void Terminal::on_draw()
	{

	}

	void Terminal::on_destroy()
	{
		XDestroyWindow(dpy, (Window)handle);
	}
}

#endif
