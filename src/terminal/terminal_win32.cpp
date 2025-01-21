
#ifdef win32
#include "terminal.h"

#include <commctrl.h>
#include <stdexcept>

#include <chrono>
#include <cstdint>
#include <iostream>

#define WINDOW_CLASS (LPCWSTR)"terminal_logger"

namespace LOG
{
	Terminal::Terminal() :
		is_running(true)
	{
		std::unique_lock<std::mutex> lk(m);
		window_thread = std::thread(&Terminal::WindowThread, this);
		cv.wait(lk);
		message_thread = std::thread(&Terminal::MessageThread, this);
	}

	Terminal::~Terminal()
	{
		is_running = false;
		window_thread.join();
		message_thread.join();
	}



	void Terminal::WindowThread(void* _data)
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

		data->window_handle = CreateWindowExW(
			WS_EX_COMPOSITED, 
			WINDOW_CLASS, WINDOW_CLASS, 
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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

		}
	}

	void Terminal::MessageThread(void* _data)
	{
		Terminal* data = (Terminal*)_data;

		while (data->is_running) {
			std::unique_lock<std::mutex> lk(data->m);
			data->cv.wait(lk, [data] {return !data->q.empty(); });

			if (!data->q.empty()) {
				std::string m = "";
				while (!data->q.empty()) {
					m += data->dequeue();
					m += "\r\n";
				}
				if (m.size() > data->buffer_size) {
					m.erase(0, m.size() - data->buffer_size);
				}

				SendMessageA((HWND)data->output_handle, WM_SETREDRAW, FALSE, NULL);
				LockWindowUpdate((HWND)data->output_handle);

				int length = GetWindowTextLength((HWND)data->output_handle);

				if (length + m.size() > data->buffer_size) {
					SendMessageA((HWND)data->output_handle, EM_SETSEL, 0, m.size());
					SendMessageA((HWND)data->output_handle, EM_REPLACESEL, TRUE, (LPARAM)"");
				}

				SendMessageA((HWND)data->output_handle, EM_SETSEL, length, length);
				SendMessageA((HWND)data->output_handle, EM_REPLACESEL, TRUE, (LPARAM)m.c_str());
				SendMessageA((HWND)data->output_handle, EM_SCROLL, SB_BOTTOM, 0);

				LockWindowUpdate(NULL);
				SendMessageA((HWND)data->output_handle, WM_SETREDRAW, TRUE, NULL);


				/*
				int i = 0;
				while (!data->q.empty()) {
					std::string m = "";
					m += data->dequeue();
					m += "\r\n";

					if (strlen(buffer) + m.size() > bufferSize) {
						size_t excessLength = strlen(buffer) + m.size() + 2 - bufferSize;
						memmove(buffer, buffer + excessLength, bufferSize - excessLength);
						buffer[bufferSize - excessLength] = '\0';
					}

					strncat(buffer, m.c_str(), bufferSize - strlen(buffer) - 1);
				}

				SendMessageA((HWND)data->output_handle, WM_SETREDRAW, FALSE, 0);
				LockWindowUpdate((HWND)data->output_handle);

				SetWindowTextA((HWND)data->output_handle, buffer);
				SendMessageA((HWND)data->output_handle, EM_SCROLL, SB_BOTTOM, 0);

				LockWindowUpdate(NULL);
				SendMessageA((HWND)data->output_handle, WM_SETREDRAW, TRUE, 0);
				*/
			}

		}
	}



	LRESULT Terminal::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm)
	{
		Terminal* data = reinterpret_cast<Terminal*>(::GetWindowLongPtr(wnd, GWLP_USERDATA));
		static HDC hdcMem = NULL;
		static HBITMAP hBitmap = NULL;

		switch (msg) {
			case WM_NCCREATE: {

				data = static_cast<Terminal*>(reinterpret_cast<LPCREATESTRUCT>(lpm)->lpCreateParams);
				SetWindowLongPtrA(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));

				break;
			}
			case WM_CREATE: {

				RECT rect;
				GetClientRect(wnd, &rect);

				data->output_handle = CreateWindowExA(
					WS_EX_TRANSPARENT,
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
				hf = CreateFontA(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, CLIP_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, FF_DONTCARE, (LPCSTR)"Consolas");
				SendMessageA((HWND)data->output_handle, WM_SETFONT, (LPARAM)hf, true);
				SendMessageA((HWND)data->output_handle, EM_SETLIMITTEXT, data->buffer_size, 0);

				break;
			}
			case WM_SIZE: {
				SetWindowPos((HWND)data->output_handle, 0, 0, 0, LOWORD(lpm), HIWORD(lpm), SWP_NOZORDER);
				SendMessageA((HWND)data->output_handle, EM_SCROLLCARET, 0, 0);

				break;
			}
		}

		return DefWindowProcA(wnd, msg, wpm, lpm);
	}
}

#endif
