
#ifdef win32
#include "terminal.h"

#include <windows.h>
#include <commctrl.h>
#include <stdexcept>
#include <iostream>

#define WINDOW_CLASS (LPCWSTR)"terminal_logger"

namespace LOG
{
	static LRESULT CALLBACK HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm)
	{
		switch (msg) {
			case WM_CREATE: {

				RECT rect;
				GetClientRect(wnd, &rect);

				HWND edit = CreateWindowEx(
					LVS_EX_DOUBLEBUFFER,
					(LPCSTR)"EDIT",
					NULL,
					WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
					0, 0, rect.right - rect.left, rect.bottom - rect.top,
					wnd,
					NULL,
					(HINSTANCE)GetWindowLongPtr(wnd, GWLP_HINSTANCE),
					NULL
				);

				HFONT hf;
				hf = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, CLIP_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, FF_DONTCARE, (LPCSTR)"Consolas");
				SendMessage(edit, WM_SETFONT, (LPARAM)hf, true);
				SetWindowLongPtr(wnd, GWLP_USERDATA, (LONG_PTR)edit);

				break;
			}
			case WM_SIZE: {
				HWND edit = (HWND)GetWindowLongPtr(wnd, GWLP_USERDATA);
				SetWindowPos(edit, 0, 0, 0, LOWORD(lpm), HIWORD(lpm), SWP_NOZORDER);

				RECT rect;
				SendMessage(edit, EM_GETRECT, 0, (LPARAM)&rect);
				rect.left += 10;
				rect.top += 10;
				rect.right -= 10;
				rect.bottom -= 10;
				SendMessage(edit, EM_SETRECT, 0, (LPARAM)&rect);

				break;
			}
			case WM_SETTEXT: {
				HWND edit = (HWND)GetWindowLongPtr(wnd, GWLP_USERDATA);

				int length = GetWindowTextLength(edit);
				SendMessage(edit, EM_SETSEL, length, length);

				std::string m((const char*)wpm);
				m += "\r\n";
				SendMessage(edit, EM_REPLACESEL, TRUE, (LPARAM)m.c_str());
			}
		}

		return DefWindowProc(wnd, msg, wpm, lpm);
	}



	Terminal::Terminal()
	{
		is_running = true;
		thread_handle = std::thread(&Terminal::ThreadEntry, this);
	}

	Terminal::~Terminal()
	{
		is_running = false;
		thread_handle.join();
	}



	void Terminal::ThreadEntry(void* _data)
	{
		Terminal* data = (Terminal*)_data;

		WNDCLASSEXW wcex = {};

		if (!GetClassInfoExW(GetModuleHandleW(NULL), WINDOW_CLASS, &wcex)) {
			wcex.cbSize = sizeof(WNDCLASSEXW);
			wcex.lpfnWndProc = HandleMessage;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = GetModuleHandleW(NULL);
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcex.lpszClassName = WINDOW_CLASS;
			wcex.style = CS_BYTEALIGNWINDOW | CS_DBLCLKS;

			if (!RegisterClassExW(&wcex)) {
				throw std::runtime_error("Window class failed to register");
			}
		}

		data->window_handle = CreateWindowExW(0, WINDOW_CLASS, WINDOW_CLASS, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			600, 400,
			0, (HMENU)0, GetModuleHandleW(NULL), (void*)data);
		if (!data->window_handle) {
			throw std::runtime_error("Window creation failed");
		}

        MSG msg = {};
		while (data->is_running) {
			if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (!data->q.empty()) {
				std::string m = data->dequeue();
				SendMessageA((HWND)data->window_handle, WM_SETTEXT, (WPARAM)m.c_str(), 0);
			}
		}
	}
}

#endif
