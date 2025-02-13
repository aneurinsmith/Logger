
#ifdef win32
#include "console.h"

namespace LOG
{
	unsigned int Console::get_width()
	{
		RECT client_rc;
		GetClientRect((HWND)handle, &client_rc);
		return (unsigned int)client_rc.right;
	}

	unsigned int Console::get_height()
	{
		RECT client_rc;
		GetClientRect((HWND)handle, &client_rc);
		return (unsigned int)client_rc.bottom;
	}

	LRESULT CALLBACK Console::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm)
	{
		Console* data = reinterpret_cast<Console*>(::GetWindowLongPtr(wnd, GWLP_USERDATA));

		switch (msg) {
		case WM_NCCREATE:
			data = static_cast<Console*>(reinterpret_cast<LPCREATESTRUCT>(lpm)->lpCreateParams);
			SetWindowLongPtrA(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
			break;
		case WM_ERASEBKGND:
			return TRUE;

		case WM_SIZE:
			data->on_size();
			SetScrollPos((HWND)data->handle, SB_VERT, data->get_scrollPos(), TRUE);
			SetScrollRange((HWND)data->handle, SB_VERT, 0, 10000, FALSE);
			InvalidateRect((HWND)data->handle, NULL, TRUE);
			break;
		case WM_PAINT:
			data->on_draw();
			break;
		case WM_DESTROY:
			data->on_destroy();
			break;

		case WM_VSCROLL:
			switch (LOWORD(wpm)) {
			case SB_LINEUP:
				data->scroll_up();
				break;
			case SB_LINEDOWN:
				data->scroll_down();
				break;
			case SB_THUMBTRACK:
			case SB_THUMBPOSITION:
				data->set_scrollPos(HIWORD(wpm));
				break;
			}
			SetScrollPos((HWND)data->handle, SB_VERT, data->get_scrollPos(), TRUE);
			SetScrollRange((HWND)data->handle, SB_VERT, 0, 10000, FALSE);
			InvalidateRect((HWND)data->handle, NULL, TRUE);
			break;
		case WM_MOUSEWHEEL:
			if (GET_WHEEL_DELTA_WPARAM(wpm) > 0) {
				data->scroll_up();
			}
			else {
				data->scroll_down();
			}
			SetScrollPos((HWND)data->handle, SB_VERT, data->get_scrollPos(), TRUE);
			SetScrollRange((HWND)data->handle, SB_VERT, 0, 10000, FALSE);
			InvalidateRect((HWND)data->handle, NULL, TRUE);

			break;

		case WM_ENTERSIZEMOVE:
			SetTimer((HWND)data->handle, 0, 10, NULL);
			return FALSE;
		case WM_APP:
		case WM_TIMER:
			SetScrollPos((HWND)data->handle, SB_VERT, data->get_scrollPos(), TRUE);
			SetScrollRange((HWND)data->handle, SB_VERT, 0, 10000, FALSE);
			InvalidateRect((HWND)data->handle, NULL, TRUE);
			data->isUpdateScheduled = false;
			break;
		case WM_EXITSIZEMOVE:
			KillTimer((HWND)data->handle, 0);
			return FALSE;
		}

		return DefWindowProcA(wnd, msg, wpm, lpm);
	}

	void Console::init()
	{
		isUpdateScheduled = false;
		WNDCLASSEXA wcea = {};

		if (!GetClassInfoExA(GetModuleHandleA(NULL), (LPCSTR)WINDOW_NAME, &wcea)) {
			wcea.cbSize = sizeof(WNDCLASSEXW);
			wcea.lpfnWndProc = HandleMessage;
			wcea.cbClsExtra = 0;
			wcea.cbWndExtra = 0;
			wcea.hInstance = GetModuleHandleA(NULL);
			wcea.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcea.lpszClassName = (LPCSTR)WINDOW_NAME;
			wcea.style = CS_BYTEALIGNWINDOW | CS_DBLCLKS;

			if (!RegisterClassExA(&wcea)) {
				throw std::runtime_error("Window class failed to register");
			}
		}

		handle = CreateWindowExA(0,
			(LPCSTR)WINDOW_NAME, (LPCSTR)WINDOW_NAME,
			WS_OVERLAPPEDWINDOW |  WS_VISIBLE | WS_VSCROLL,
			CW_USEDEFAULT, CW_USEDEFAULT,
			600, 400,
			0, (HMENU)0, GetModuleHandleA(NULL), (void*)this);
		if (!handle) {
			throw std::runtime_error("Window creation failed");
		}
	}

	void Console::run()
	{
		MSG msg = {};
		while (is_running) {
			if (GetMessageA(&msg, NULL, 0, 0)) {
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}
		}
	}

	void Console::update()
	{
		if (!isUpdateScheduled.exchange(true)) {
			PostMessageA((HWND)handle, WM_APP, 0, 0);
		}
	}



	void draw_text(HDC hdc, const std::string& text, 
		int& x, int& y, int width, int height, 
		COLORREF textColor = 0xCCCCCC, COLORREF bgColor = TRANSPARENT)
	{
		SetTextColor(hdc, textColor);
		if (bgColor != TRANSPARENT) {
			SetBkMode(hdc, OPAQUE);
			SetBkColor(hdc, bgColor);
		}
		else {
			SetBkMode(hdc, TRANSPARENT);
		}

		for (int i = 0; i < text.size() && y <= (int)(height / 16);) {
			int chars_to_draw = std::min((int)(text.size() - i), (int)((width / 8) - x));
			std::string msg_substr = text.substr(i, chars_to_draw);

			if (x >= width / 8) {
				x = 0;
				y++;
			}
			if (y > 0) {
				RECT msg_rc = { x * 8, (y - 1) * 16,0,0 };
				DrawTextA(hdc, (LPCSTR)msg_substr.c_str(), -1, &msg_rc, DT_TOP | DT_NOCLIP);
			}
			x += chars_to_draw;
			i += chars_to_draw;
		}
	}

	void Console::on_draw()
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint((HWND)handle, &ps);
		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP memBM = CreateCompatibleBitmap(hdc, ps.rcPaint.right, ps.rcPaint.bottom);
		SelectObject(memDC, memBM);

		// Set background
		HBRUSH brush = CreateSolidBrush(0x0C0C0C);
		FillRect(memDC, &ps.rcPaint, brush);

		// Create font
		int dpi = GetDpiForWindow((HWND)handle);
		HFONT hf = CreateFontA((14 * dpi) / 72, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, CLIP_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, FF_DONTCARE, (LPCSTR)"Consolas");
		SelectObject(memDC, hf);

		// Render messages
		m.lock();
		if (!msgs.empty()) {
			int y = 1 - linePos;
			for (auto it = msgs.begin() + msgsPos; it != msgs.end() && y <= (int)(get_height()/16); ++it) {
				
				Message m = *it;
				std::string lvl_text = "";
				std::string tab = std::string(8 - (m.ts.length() % 8), ' ');
				if (m.lvl != LOG::NONE) m.msg = "  " + m.msg;
				COLORREF textColor = 0xCCCCCC, bgColor = TRANSPARENT;

				int x = 0;
				draw_text(memDC, m.ts, x, y, get_width(), get_height(), 0x767676);
				draw_text(memDC, tab, x, y, get_width(), get_height());

				switch (m.lvl) {
				case LOG::TRACE:
					lvl_text = "[TRACE]";
					textColor = 0x767676;
					break;
				case LOG::DEBUG:
					lvl_text = "[DEBUG]";
					textColor = 0xF2F2F2;
					break;
				case LOG::INFO:
					lvl_text = " [INFO]";
					textColor = 0xF2F2F2;
					bgColor = 0xDD963A;
					break;
				case LOG::WARN:
					lvl_text = " [WARN]";
					textColor = 0x0C0C0C;
					bgColor = 0x009CC1;
					break;
				case LOG::ERROR:
					lvl_text = "[ERROR]";
					textColor = 0xCCCCCC;
					bgColor = 0x5648E7;
					break;
				case LOG::FATAL:
					lvl_text = "[FATAL]";
					textColor = 0xCCCCCC;
					bgColor = 0x1F0FC5;
					break;
				}

				draw_text(memDC, lvl_text, x, y, get_width(), get_height(), textColor, bgColor);
				draw_text(memDC, m.msg, x, y, get_width(), get_height(), 0xCCCCCC);
				y++;
			}
		}
		m.unlock();

		// Swap buffers
		BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, memDC, 0, 0, SRCCOPY);
		EndPaint((HWND)handle, &ps);

		DeleteObject(hf);
		DeleteObject(brush);
		DeleteDC(memDC);
		DeleteObject(memBM);
	}

	void Console::on_destroy()
	{

	}
}

#endif
