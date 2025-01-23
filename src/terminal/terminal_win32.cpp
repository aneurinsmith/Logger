
#ifdef win32
#include "terminal.h"

#include <commctrl.h>
#include <stdexcept>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <algorithm>

using std::max;
using std::min;

#define WINDOW_CLASS (LPCWSTR)"terminal_logger"

namespace LOG
{
	Terminal::Terminal() :
		is_running(true)
	{
		buffer = new char[8192];
		buffer[0] = '\0';

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

		data->window_handle = CreateWindowExW(0,
			WINDOW_CLASS, WINDOW_CLASS, 
			WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_VSCROLL,
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
	}


	
	static bool isWithin = true;
	static int msgsPos = 0;
	static int linePos = 0;
	LRESULT Terminal::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm)
	{
		Terminal* data = reinterpret_cast<Terminal*>(::GetWindowLongPtr(wnd, GWLP_USERDATA));

		switch (msg) {
			case WM_ERASEBKGND: {
				return TRUE;
			}
			case WM_NCCREATE: {
				data = static_cast<Terminal*>(reinterpret_cast<LPCREATESTRUCT>(lpm)->lpCreateParams);
				SetWindowLongPtrA(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));

				break;
			}
			case WM_SIZE: {
				InvalidateRect(wnd, NULL, true);

				RECT rc;
				GetClientRect(wnd, &rc);

				/*
				SCROLLINFO si;
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_RANGE | SIF_PAGE;
				si.nMin = 0;
				si.nMax = data->MAX_QUEUE - (rc.bottom / 18);
				si.nPage = 1;
				SetScrollInfo(wnd, SB_VERT, &si, TRUE);
				*/

				break;
			}
			case WM_VSCROLL: {

				HDC hdc = GetDC(wnd);
				HFONT hf = CreateFontA(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, CLIP_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, FF_DONTCARE, (LPCSTR)"Consolas");
				SelectObject(hdc, hf);
				RECT topMsgRect;
				GetClientRect(wnd, &topMsgRect);

				auto it = data->msgs.begin() + msgsPos;
				std::string msg = *it;
				int topMsgHeight = DrawTextA(hdc, (LPCSTR)msg.c_str(), -1, &topMsgRect, DT_CALCRECT | DT_WORDBREAK);

				switch (LOWORD(wpm)) {
					case SB_LINEUP: {
						if (linePos > 0) {
							linePos--;
						}
						else {
							if (msgsPos > 0) {
								msgsPos--;
								linePos = (DrawTextA(hdc, (LPCSTR)msg.c_str(), -1, &topMsgRect, DT_CALCRECT | DT_WORDBREAK) - 18) / 18;
							}
						}
						break;
					}
					case SB_LINEDOWN: {
						if (linePos < (topMsgHeight - 18) / 18) {
							linePos++;
						}
						else {
							if (msgsPos < data->msgs.size() - 1) {
								msgsPos++;
								linePos = 0;
							}
						}
						break;
					}
				}

				//SetScrollPos(wnd, SB_VERT, sp, TRUE);
				InvalidateRect(wnd, NULL, TRUE);

				break;
			}
			case WM_MOUSEWHEEL: {

				int delta = GET_WHEEL_DELTA_WPARAM(wpm);
				HDC hdc = GetDC(wnd);
				HFONT hf = CreateFontA(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, CLIP_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, FF_DONTCARE, (LPCSTR)"Consolas");
				SelectObject(hdc, hf);
				RECT topMsgRect;
				GetClientRect(wnd, &topMsgRect);

				auto it = data->msgs.begin() + msgsPos;
				std::string msg = *it;
				int topMsgHeight = DrawTextA(hdc, (LPCSTR)msg.c_str(), -1, &topMsgRect, DT_CALCRECT | DT_WORDBREAK);

				if (delta > 0) {
					if (linePos > 0) {
						linePos--;
					}
					else {
						if (msgsPos > 0) {
							msgsPos--;
							linePos = (DrawTextA(hdc, (LPCSTR)msg.c_str(), -1, &topMsgRect, DT_CALCRECT | DT_WORDBREAK) - 18) / 18;
						}
					}
				}
				else if (delta < 0) {
					if (linePos < (topMsgHeight - 18) / 18) {
						linePos++;
					}
					else {
						if (msgsPos < data->msgs.size() - 1) {
							msgsPos++;
							linePos = 0;
						}
					}
				}

				//SetScrollPos(wnd, SB_VERT, sp, TRUE);
				InvalidateRect(wnd, NULL, TRUE);

				return FALSE;
			}
			case WM_PAINT: {

				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(wnd, &ps);
				HDC memDC = CreateCompatibleDC(hdc);
				HBITMAP memBM = CreateCompatibleBitmap(hdc, ps.rcPaint.right, ps.rcPaint.bottom);
				SelectObject(memDC, memBM);

				// Set background
				HBRUSH brush = CreateSolidBrush(0x0C0C0C);
				FillRect(memDC, &ps.rcPaint, brush);

				// Create font
				HFONT hf = CreateFontA(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, CLIP_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, FF_DONTCARE, (LPCSTR)"Consolas");
				SelectObject(memDC, hf);

				SetBkMode(memDC, TRANSPARENT);
				SetTextColor(memDC, 0xCCCCCC);



				data->m.lock();
				if (!data->msgs.empty()) {
					RECT topMsgRect;
					GetClientRect(wnd, &topMsgRect);
					topMsgRect.top -= linePos * 18;

					for (auto it = data->msgs.begin() + msgsPos; it != data->msgs.end(); ++it) {

						RECT rc_size = topMsgRect;
						std::string msg = *it;
						int height = DrawTextA(memDC, (LPCSTR)msg.c_str(), -1, &rc_size, DT_CALCRECT | DT_WORDBREAK);
						DrawTextA(memDC, (LPCSTR)msg.c_str(), -1, &rc_size, DT_WORDBREAK);

						topMsgRect.top += height;
						topMsgRect.bottom += height;

						if (rc_size.bottom > ps.rcPaint.bottom - 18) break;
					}
				}
				data->m.unlock();



				// Swap buffers
				BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, memDC, 0, 0, SRCCOPY);
				EndPaint(wnd, &ps);

				DeleteDC(memDC);
				DeleteObject(memBM);

				return FALSE;
			}
		}

		return DefWindowProcA(wnd, msg, wpm, lpm);
	}
}

#endif
