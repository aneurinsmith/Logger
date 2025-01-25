
#ifdef win32__
#include "terminal.h"

#include <gdk/gdkwin32.h>
#include <dwmapi.h>
#include <Uxtheme.h>

namespace LOG
{
	static void on_window_show(GtkWidget* window, gpointer data)
	{

		GdkWindow* gdk_window = gtk_widget_get_window(window);
		HWND hwnd = (HWND)gdk_win32_window_get_handle(gdk_window);

		BOOL value = TRUE;
		DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));

	}

	Terminal::Terminal()
	{
		gtk_init(0, 0);

		GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(window), "Logger Window");
		g_signal_connect(window, "realize", G_CALLBACK(on_window_show), NULL);

		gtk_widget_show_all(window);



		gtk_main();
	}

	Terminal::~Terminal()
	{

	}



	void Terminal::WindowThread(void* data)
	{

	}
}
#endif