
#ifdef win32
#include "terminal.h"
#include <windows.h>
#include <stdexcept>

#define WINDOW_CLASS (LPCWSTR)"terminal_logger"

namespace LOG
{
	static LRESULT CALLBACK DefProc(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm)
	{
		return DefWindowProc(wnd, msg, wpm, lpm);
	}

	Terminal::Terminal()
	{
		WNDCLASSEXW wcex = {};

		if (!GetClassInfoExW(GetModuleHandleW(NULL), WINDOW_CLASS, &wcex)) {
			wcex.cbSize = sizeof(WNDCLASSEXW);
			wcex.lpfnWndProc = DefProc;
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

		window_handle = CreateWindowExW(0, WINDOW_CLASS, WINDOW_CLASS, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 
			CW_USEDEFAULT, CW_USEDEFAULT, 
			0, (HMENU)0, GetModuleHandleW(NULL), (void*)this);
		if (!window_handle) {
			throw std::runtime_error("Window creation failed");
		}

		ShowWindow((HWND)window_handle, SW_SHOW);
	}

	Terminal::~Terminal()
	{

	}
}

#endif
