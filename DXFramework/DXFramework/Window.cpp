#include "Window.h"
#include "WinUser.h"

LRESULT Window::msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* window = 0;

	switch (msg)
	{
	case WM_CREATE:
		window = (Window*)(getWindowLong(hwnd));
		return 0;

	case WM_INPUT:
		return 0;

	case WM_GETMINMAXINFO:
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = window->getMinSize().x;
		lpMMI->ptMinTrackSize.y = window->getMinSize().y;
		return 0;

	case WM_SIZE:
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SYSCOMMAND:
		if (wParam == SC_KEYMENU && (lParam >> 16) <= 0) return 0;
		break;

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::Window(HINSTANCE instance, LPCSTR title, int width, int height) : Window(instance, (title), { width, height }) {}

Window::Window(HINSTANCE instance, LPCSTR title, const XMINT2& size) : title_(title), size_(size)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = instance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "Window";
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = msgProc;

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, size.x, size.y };

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	handle_ = CreateWindowEx(NULL,
		"Window",
		"DirectX Framework",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL, NULL, instance, NULL);

	::SetWindowLong(handle_, -21, long(this));

	if (!handle_)
		throw std::invalid_argument("Window Not Created");

	ShowWindow(handle_, SW_SHOW);
	UpdateWindow(handle_);
}

void Window::getMousePosition(POINT* p)
{
	*p = { mouse_position_.x, mouse_position_.y };
}

void Window::setTitle(LPCSTR title)
{
	title_ = title;
}

void Window::setSize(int width, int height)
{
	size_ = { width, height };
}

void Window::setMinSize(int width, int height)
{
	min_size_ = { width, height };
}

void Window::setResizable(bool resizable)
{
	resizable_ = resizable_;
}

void Window::setMaximizable(bool maximizable)
{
	maximizable_ = maximizable_;
}

Window* Window::getWindowLong(HWND hwnd)
{
	return (Window*)GetWindowLong(hwnd, -21);
}

LPCSTR Window::getTitle()
{
	return title_;
}

XMINT2 Window::getSize()
{
	return size_;
}

XMINT2 Window::getMinSize()
{
	return min_size_;
}

bool Window::isResizable()
{
	return resizable_;
}

bool Window::isMaximizable()
{
	return maximizable_;
}

HWND Window::getHandle()
{
	return handle_;
}

