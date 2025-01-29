
#ifdef win32
#include "terminal.h"

namespace LOG
{
	LRESULT CALLBACK Terminal::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm)
	{
		Terminal* data = reinterpret_cast<Terminal*>(::GetWindowLongPtr(wnd, GWLP_USERDATA));

		switch (msg) {
		case WM_NCCREATE:
			data = static_cast<Terminal*>(reinterpret_cast<LPCREATESTRUCT>(lpm)->lpCreateParams);
			SetWindowLongPtrA(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
			break;
		case WM_SIZE: 
			InvalidateRect((HWND)data->handle, NULL, TRUE);
			break;
		case WM_MOUSEWHEEL: 
			data->on_scroll(GET_WHEEL_DELTA_WPARAM(wpm));
			break;
		case WM_PAINT:
			data->on_draw();
			break;
		case WM_ERASEBKGND:
			return TRUE;
		case WM_DESTROY:
			data->on_destroy();
			break;
		}

		return DefWindowProcA(wnd, msg, wpm, lpm);
	}

	void Terminal::init()
	{
		WNDCLASSEXW wcex = {};
		
		if (!GetClassInfoExW(GetModuleHandleW(NULL), (LPCWSTR)WINDOW_NAME, &wcex)) {
			wcex.cbSize = sizeof(WNDCLASSEXW);
			wcex.lpfnWndProc = HandleMessage;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = GetModuleHandleW(NULL);
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcex.lpszClassName = (LPCWSTR)WINDOW_NAME;
			wcex.style = CS_BYTEALIGNWINDOW | CS_DBLCLKS;

			if (!RegisterClassExW(&wcex)) {
				throw std::runtime_error("Window class failed to register");
			}
		}

		handle = CreateWindowExW(0,
			(LPCWSTR)WINDOW_NAME, (LPCWSTR)WINDOW_NAME,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_VSCROLL,
			CW_USEDEFAULT, CW_USEDEFAULT,
			600, 400,
			0, (HMENU)0, GetModuleHandleW(NULL), (void*)this);
		if (!handle) {
			throw std::runtime_error("Window creation failed");
		}
	}

	void Terminal::run()
	{
		MSG msg = {};
		while (is_running) {
			if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	void Terminal::update()
	{
		MSG msg;
		if (!PeekMessageA(&msg, (HWND)handle, WM_SIZE, WM_SIZE, PM_NOREMOVE)) {
			PostMessageA((HWND)handle, WM_SIZE, 0, 0);
		}
	}



	void Terminal::on_scroll(int delta)
	{
		// Get window geometry
		RECT client_rc;
		GetClientRect((HWND)handle, &client_rc);
		unsigned int width = client_rc.right, height = client_rc.bottom;

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
		RECT client_rc;
		GetClientRect((HWND)handle, &client_rc);
		unsigned int width = client_rc.right, height = client_rc.bottom;

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

		SetBkMode(memDC, TRANSPARENT);
		SetTextColor(memDC, 0xCCCCCC);



		m.lock();
		if (!msgs.empty()) {
			int y = 1 - linePos;
			for (auto it = msgs.begin() + msgsPos; it != msgs.end() && y <= (int)(height / 16); ++it) {

				std::string msg = *it;
				for (int x = 0; x < msg.size() && y <= (int)(height / 16); x += (width / 8), y++) {
					std::string msg_substr = msg.substr(x, (width / 8));
					if (y > 0) {
						RECT msg_rc = { 0,(y-1)*16,0,0 };
						DrawTextA(memDC, (LPCSTR)msg_substr.c_str(), -1, &msg_rc, DT_TOP | DT_NOCLIP);
					}
				}

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

	void Terminal::on_destroy()
	{

	}
}

#endif
