
#ifdef win32
#include "terminal.h"

#include <pango/pangocairo.h>
#include <gdk/gdkwin32.h>
#include <dwmapi.h>
#include <Uxtheme.h>

namespace LOG
{
	void Terminal::on_activate(GtkApplication* app, gpointer _data)
	{
		Terminal* data = (Terminal*)_data;

		data->window = gtk_application_window_new(app);
		gtk_window_set_title(GTK_WINDOW(data->window), "Logger Window");
		data->drawable = gtk_drawing_area_new();
		gtk_container_add(GTK_CONTAINER(data->window), data->drawable);

		GdkColor bkgColor;
		gdk_color_parse("#0C0C0C", &bkgColor);
		gtk_widget_modify_bg(data->window, GTK_STATE_NORMAL, &bkgColor);

		g_signal_connect(data->window, "realize", G_CALLBACK(on_realise), data);
		g_signal_connect(data->drawable, "draw", G_CALLBACK(on_draw), data);

		gtk_widget_show_all(data->window);
		data->cv.notify_all();
		gtk_main();
	}

	void Terminal::on_realise(GtkWidget* wnd, gpointer _data)
	{
		GdkWindow* gdk_window = gtk_widget_get_window(wnd);
		HWND hwnd = (HWND)gdk_win32_window_get_handle(gdk_window);

		BOOL value = TRUE;
		DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
	}

	gboolean Terminal::on_draw(GtkWidget* wnd, cairo_t* cr, gpointer _data)
	{
		Terminal* data = (Terminal*)_data;

		cairo_move_to(cr, 0, 2);
		cairo_set_source_rgb(cr, 0xCC, 0xCC, 0xCC);

		// Create font and text attributes
		PangoLayout* layout = pango_cairo_create_layout(cr);
		PangoFontDescription* desc = pango_font_description_from_string("Consolas 11");
		pango_layout_set_font_description(layout, desc);
		pango_font_description_free(desc);
		pango_layout_set_width(layout, 100000);
		pango_layout_set_wrap(layout, PANGO_WRAP_CHAR);

		if (!data->msgs.empty()) {
			pango_layout_set_text(layout, data->msgs.back().c_str(), -1);
		}

		// Swap buffers
		pango_cairo_show_layout(cr, layout);

		return true;
	}
}

#endif
