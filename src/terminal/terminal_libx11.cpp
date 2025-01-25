
#ifdef libx11
#include "terminal.h"

namespace LOG
{
	void Terminal::on_realise(GtkWidget* wnd, gpointer _data)
	{

	}

	gboolean Terminal::on_draw(GtkWidget* wnd, cairo_t* cr, gpointer _data)
	{
		Terminal* data = (Terminal*)_data;

		PangoLayout* layout = pango_cairo_create_layout(cr);
		PangoFontDescription* desc;

		if (data->msgs.empty()) {
			cairo_move_to(cr, 0, 10);
			pango_layout_set_text(layout, "Text", -1);
			desc = pango_font_description_from_string("Consolas 11");
			pango_layout_set_font_description(layout, desc);
			pango_font_description_free(desc);

			pango_cairo_show_layout(cr, layout);
		}

		return true;
	}
}

#endif
