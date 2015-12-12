#include "stdafx.h"
#include "Window.h"

#define IsMenuActiveByAlt(lParam) ((lParam >> 16) <= 0)

bool Window::initialized = false;
UINT Window::windowCount = 0;

LRESULT Window::msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window *window = getWindow(hwnd);

	switch (msg)
	{
	case WM_CREATE:
		break;

	case WM_INPUT:
		break;

	case WM_GETMINMAXINFO:
	{
		if (!window)
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
		mmi->ptMinTrackSize.x = window->minWndSize.x;
		mmi->ptMinTrackSize.y = window->minWndSize.y;
	} break;

	case WM_SIZE:
		break;

	case WM_DESTROY:
		window->handle = nullptr;

		if (--windowCount == 0)
		{
			PostQuitMessage(0);
		}

		break;

	case WM_SYSCOMMAND:
		if (wParam == SC_KEYMENU && IsMenuActiveByAlt(lParam)) return 0;
		return DefWindowProc(hwnd, msg, wParam, lParam);

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

Window::Window(LPCSTR title, int width, int height)
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(nullptr);

	if (!initialized)
	{
		WNDCLASSEX wcex;
		ZeroMemory(&wcex, sizeof(wcex));

		wcex.cbSize = sizeof(wcex);
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wcex.hIconSm = wcex.hIcon;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wcex.lpszClassName = "Window";
		wcex.style = CS_VREDRAW | CS_HREDRAW;
		wcex.lpfnWndProc = msgProc;

		BF(RegisterClassEx(&wcex));

		initialized = true;
	}

	RECT wr = { 0, 0, width, height };

	BF(AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE));

	handle = CreateWindow("Window",
						  title,
						  WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  wr.right - wr.left,
						  wr.bottom - wr.top,
						  nullptr,
						  nullptr,
						  hInstance,
						  nullptr);

	BF(handle);

	SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)this);

	ShowWindow(handle, SW_SHOW);

	windowCount++;
}


const XMINT2& Window::getMousePosition()
{
	return mousePosition;
}

void Window::setTitle(LPCSTR title)
{
	BF(SetWindowText(handle, title));
}

void Window::setSize(int width, int height)
{
	BF(SetWindowPos(handle, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER));
}

void Window::setMinSize(int width, int height)
{
	LONG style = GetWindowLong(handle, GWL_STYLE);

	RECT wr = { 0, 0, width, height };
	BF(AdjustWindowRect(&wr, style, FALSE));

	minWndSize.x = wr.right - wr.left;
	minWndSize.y = wr.bottom - wr.top;

	minSize.x = width;
	minSize.y = height;
}

void Window::setResizable(bool resizable)
{
	LONG style = GetWindowLong(handle, GWL_STYLE);

	if (resizable)
		style |= WS_THICKFRAME;
	else
		style &= ~WS_THICKFRAME;

	BF(SetWindowLong(handle, GWL_STYLE, style));
}

void Window::setMaximizable(bool maximizable)
{
	LONG style = GetWindowLong(handle, GWL_STYLE);

	if (maximizable)
		style |= WS_MAXIMIZEBOX;
	else
		style &= ~WS_MAXIMIZEBOX;

	BF(SetWindowLong(handle, GWL_STYLE, style));
}

void Window::setPosition(XMINT2 position)
{
	BF(SetWindowPos(handle, nullptr, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER));
}

Window* Window::getWindow(HWND hwnd)
{
	return (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

const std::string& Window::getTitle()
{
	static std::string strTitle;

	int titleLen = GetWindowTextLength(handle) + 1;

	char *title = new char[titleLen];
	GetWindowText(handle, title, titleLen);

	strTitle = title;
	delete title;

	return strTitle;
}

const XMINT2& Window::getSize()
{
	static XMINT2 size;

	RECT cr;
	BF(GetClientRect(handle, &cr));

	size.x = cr.right - cr.left;
	size.y = cr.bottom - cr.top;

	return size;
}

const XMINT2& Window::getMinSize()
{
	return minSize;
}

const XMINT2& Window::getPosition()
{
	static XMINT2 position;

	RECT wr;
	BF(GetWindowRect(handle, &wr));

	position.x = wr.left;
	position.y = wr.top;

	return position;
}

bool Window::isResizable()
{
	LONG style = GetWindowLong(handle, GWL_STYLE);
	return (style & WS_THICKFRAME) == WS_THICKFRAME;
}

bool Window::isMaximizable()
{
	LONG style = GetWindowLong(handle, GWL_STYLE);
	return (style & WS_MAXIMIZEBOX) == WS_MAXIMIZEBOX;
}

HWND Window::getHandle()
{
	return handle;
}

