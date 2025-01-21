
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
			case WM_CREATE: {

				break;
			}
			case WM_SIZE: {
				InvalidateRect(wnd, NULL, true);

				RECT rc;
				GetClientRect(wnd, &rc);

				SCROLLINFO si;
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
				si.nMin = 0;
				si.nMax = data->MAX_QUEUE - (rc.bottom / 18);
				si.nPage = 1;
				si.nPos = GetScrollPos(wnd, SB_VERT);
				SetScrollInfo(wnd, SB_VERT, &si, TRUE);

				break;
			}
			case WM_VSCROLL: {
				int sp = GetScrollPos(wnd, SB_VERT);

				switch (LOWORD(wpm)) {
				case SB_LINEUP:
					sp = max(0, sp - 1);
					break;
				case SB_LINEDOWN:
					sp = min(data->MAX_QUEUE, sp + 1);
					break;
				case SB_THUMBTRACK:
				case SB_THUMBPOSITION:
					sp = HIWORD(wpm);
					break;
				}
				SetScrollPos(wnd, SB_VERT, sp, TRUE);
				InvalidateRect(wnd, NULL, TRUE);

				break;
			}
			case WM_MOUSEWHEEL: {

				int delta = GET_WHEEL_DELTA_WPARAM(wpm);
				int sp = GetScrollPos(wnd, SB_VERT);

				if (delta > 0) {
					sp = max(0, sp - 1);
				}
				else if (delta < 0) {
					sp = min(data->MAX_QUEUE, sp + 1);
				}
				SetScrollPos(wnd, SB_VERT, sp, TRUE);
				InvalidateRect(wnd, NULL, TRUE);

				return 0;
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

				RECT rc;
				rc.left = ps.rcPaint.left;
				rc.right = ps.rcPaint.right;
				rc.bottom = ps.rcPaint.bottom;
				rc.top = ps.rcPaint.top - (GetScrollPos(wnd, SB_VERT) * 18);

				// Draw text
				DrawTextA(memDC, (LPCSTR)data->get_msgs().c_str(), -1, &rc, DT_WORDBREAK | DT_EDITCONTROL | DT_LEFT | DT_BOTTOM);

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
