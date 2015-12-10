#include "Window.h"

LRESULT Window::msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::Window(LPCSTR title, int width, int height) : title_(title), size_({ width,height }) {}

Window::Window(LPCSTR title, const XMINT2& size) : title_(title), size_(size) {}

void Window::getMousePosition(POINT* p)
{
	*p = { mouse_position_.x,mouse_position_.y };
}

void Window::setTitle(LPCSTR title)
{
	title_ = title;
}

void Window::setSize(int width, int height)
{
	size_ = { width,height };
}

void Window::setMinSize(int width, int height)
{
	min_size_ = { width,height };
}

void Window::setResizable(bool resizable)
{
	resizable_ = resizable_;
}

void Window::setMaximizable(bool maximizable)
{
	maximizable_ = maximizable_;
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

