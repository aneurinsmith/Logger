
#ifdef win32
#include "terminal.h"
#include <windows.h>
#include <stdexcept>
#include <iostream>

#define WINDOW_CLASS (LPCWSTR)"terminal_logger"

namespace LOG
{
	static LRESULT CALLBACK HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm)
	{
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
			0, (HMENU)0, GetModuleHandleW(NULL), 0);
		if (!data->window_handle) {
			throw std::runtime_error("Window creation failed");
		}

        MSG msg = {};
		while (data->is_running) {
			if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}
		}
	}
}

#endif
