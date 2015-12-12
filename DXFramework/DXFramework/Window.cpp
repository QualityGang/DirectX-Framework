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
		mmi->ptMinTrackSize.x = window->getMinSize().x;
		mmi->ptMinTrackSize.y = window->getMinSize().y;
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
		wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
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

	SetWindowLong(handle, GWLP_USERDATA, (LONG)this);

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
	BF(SetWindowPos(handle, HWND_TOP, 0, 0, width, height, SWP_NOMOVE));
}

void Window::setMinSize(int width, int height)
{
	minSize = { width, height };
}

void Window::setResizable(bool resizable)
{
	LONG l;

	if (resizable)
		l = GetWindowLong(handle, GWL_STYLE) ^ WS_THICKFRAME;
	else
		l = GetWindowLong(handle, GWL_STYLE) | WS_THICKFRAME;

	SetWindowLong(handle, GWL_STYLE,l);
}

void Window::setMaximizable(bool maximizable)
{
	LONG l;

	if (maximizable)
		l = GetWindowLong(handle, GWL_STYLE) ^ WS_MAXIMIZEBOX;
	else
		l = GetWindowLong(handle, GWL_STYLE) | WS_MAXIMIZEBOX;

	SetWindowLong(handle, GWL_STYLE, l);
}

void Window::setPosition(XMINT2 position)
{
	BF(SetWindowPos(handle, HWND_TOP, position.x, position.y, 0, 0, SWP_NOSIZE));
}

Window* Window::getWindow(HWND hwnd)
{
	return (Window*)GetWindowLong(hwnd, GWLP_USERDATA);
}

std::string Window::getTitle()
{
	int titleLen = GetWindowTextLength(handle) + 1;

	char *title = new char[titleLen];
	GetWindowText(handle, title, titleLen);

	std::string strTitle(title);
	delete title;

	return strTitle;
}

XMINT2 Window::getSize()
{
	RECT rect;
	BF(!GetClientRect(handle, &rect));

	return{ rect.right - rect.left, rect.bottom - rect.top };
}

const XMINT2& Window::getMinSize()
{
	return minSize;
}

bool Window::isResizable()
{
}

bool Window::isMaximizable()
{

}

HWND Window::getHandle()
{
	return handle;
}

