
#ifdef win32
#include "terminal.h"

namespace LOG
{
	LRESULT CALLBACK Terminal::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm)
	{
		Terminal* data = reinterpret_cast<Terminal*>(::GetWindowLongPtr(wnd, GWLP_USERDATA));

		switch (msg) {
		case WM_CREATE:
			data = static_cast<Terminal*>(reinterpret_cast<LPCREATESTRUCT>(lpm)->lpCreateParams);
			SetWindowLongPtrA(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
			break;
		case WM_MOUSEWHEEL: 
			data->on_scroll(GET_WHEEL_DELTA_WPARAM(wpm));
			break;
		case WM_PAINT:
			data->on_draw();
			break;
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

	}



	void Terminal::on_scroll(int delta)
	{

	}

	void Terminal::on_draw()
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

		SetBkMode(memDC, TRANSPARENT);
		SetTextColor(memDC, 0xCCCCCC);



		RECT topMsgRect;
		GetClientRect((HWND)handle, &topMsgRect);
		RECT rc_size = topMsgRect;
		DrawTextA(memDC, (LPCSTR)"Hello World!", 11, &rc_size, DT_WORDBREAK);



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
