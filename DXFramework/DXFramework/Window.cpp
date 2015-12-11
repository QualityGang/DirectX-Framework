#include "Window.h"
#include "stdafx.h"

LRESULT Window::msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* window = 0;

	window = getWindow(hwnd);

	switch (msg)
	{
	case WM_CREATE:
		break;

	case WM_INPUT:
		break;

	case WM_GETMINMAXINFO:
		LPMINMAXINFO MMI = (LPMINMAXINFO)lParam;
		MMI->ptMinTrackSize.x = window->getMinSize().x;
		MMI->ptMinTrackSize.y = window->getMinSize().y;
		break;

	case WM_SIZE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SYSCOMMAND:
		if (wParam == SC_KEYMENU && IsMenuActiveByAlt) return 0;
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

Window::Window(HINSTANCE instance, LPCSTR title, int width, int height) : title(title), size({width,height})
{

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = instance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = "Window";
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = msgProc;

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, size.x, size.y };

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	handle = CreateWindowEx(NULL,
							"Window",
							title,
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							wr.right - wr.left,
							wr.bottom - wr.top,
							nullptr,
							nullptr,
							instance,
							nullptr);

	SetWindowLong(handle, GWL_USERDATA, (LONG)this);

	if (!handle)
		throw std::invalid_argument("Window Not Created");

	ShowWindow(handle, SW_SHOW);
}


XMINT2 Window::getMousePosition()
{
	return { mousePosition.x, mousePosition.y };
}

void Window::setTitle(LPCSTR title)
{
	this->title = title;
}

void Window::setSize(int width, int height)
{
	size = { width, height };
}

void Window::setMinSize(int width, int height)
{
	minSize = { width, height };
}

void Window::setResizable(bool resizable)
{
	this->resizable = resizable;
}

void Window::setMaximizable(bool maximizable)
{
	this->maximizable = maximizable;
}

Window* Window::getWindow(HWND hwnd)
{
	return (Window*)GetWindowLong(hwnd, -21);
}

LPCSTR Window::getTitle()
{
	return title;
}

XMINT2& Window::getSize()
{
	return size;
}

XMINT2& Window::getMinSize()
{
	return minSize;
}

bool Window::isResizable()
{
	return resizable;
}

bool Window::isMaximizable()
{
	return maximizable;
}

HWND Window::getHandle()
{
	return (HWND)GetModuleHandle(nullptr);
}

