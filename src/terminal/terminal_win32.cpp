
#ifdef win32
#include "terminal.h"

#include <commctrl.h>
#include <stdexcept>
#include <iostream>

#define WINDOW_CLASS (LPCWSTR)"terminal_logger"

namespace LOG
{
	Terminal::Terminal()
	{
		std::unique_lock<std::mutex> lk(m);
		is_running = true;
		thread_handle = std::thread(&Terminal::ThreadEntry, this);
		cv.wait(lk);
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

		data->cv.notify_all();

        MSG msg = {};
		while (data->is_running) {
			if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (!data->q.empty()) {
				SendMessageA((HWND)data->output_handle, WM_SETREDRAW, FALSE, NULL);
				LockWindowUpdate((HWND)data->output_handle);

				std::string m = data->dequeue();
				m += "\r\n";

				int length = GetWindowTextLength((HWND)data->output_handle);
				if (length + m.size() > 30000) {
					std::cout << length << std::endl;
					SendMessage((HWND)data->output_handle, EM_SETSEL, 0, m.size());
					SendMessage((HWND)data->output_handle, EM_REPLACESEL, TRUE, (LPARAM)"");
				}

				LockWindowUpdate(NULL);
				SendMessageA((HWND)data->output_handle, WM_SETREDRAW, TRUE, NULL);

				SendMessage((HWND)data->output_handle, EM_SETSEL, length, length);
				SendMessage((HWND)data->output_handle, EM_REPLACESEL, TRUE, (LPARAM)m.c_str());

			}
		}
	}



	LRESULT CALLBACK Terminal::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm)
	{
		Terminal* data = reinterpret_cast<Terminal*>(::GetWindowLongPtr(wnd, GWLP_USERDATA));

		switch (msg) {
			case WM_NCCREATE: {

				data = static_cast<Terminal*>(reinterpret_cast<LPCREATESTRUCT>(lpm)->lpCreateParams);
				SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));

				break;
			}
			case WM_CREATE: {

				RECT rect;
				GetClientRect(wnd, &rect);

				data->output_handle = CreateWindowEx(
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
				SendMessage((HWND)data->output_handle, WM_SETFONT, (LPARAM)hf, true);

				break;
			}
			case WM_SIZE: {
				SetWindowPos((HWND)data->output_handle, 0, 0, 0, LOWORD(lpm), HIWORD(lpm), SWP_NOZORDER);

				RECT rect;
				SendMessage((HWND)data->output_handle, EM_GETRECT, 0, (LPARAM)&rect);
				rect.left += 10;
				rect.top += 10;
				rect.right -= 10;
				rect.bottom -= 10;
				SendMessage((HWND)data->output_handle, EM_SETRECT, 0, (LPARAM)&rect);

				break;
			}
		}

		return DefWindowProc(wnd, msg, wpm, lpm);
	}
}

#endif
