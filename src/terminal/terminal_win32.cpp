
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



	void Terminal::on_draw()
	{

	}

	void Terminal::on_destroy()
	{

	}
}

#endif
