#include "stdafx.h"
#include "Window.h"

#define IsMenuActiveByAlt(lParam) ((lParam >> 16) <= 0)

#define WND_CLASSNAME "DXFrameworkClassName"

#define RID_USAGE_MOUSE 2
#define RID_USAGE_KEYBOARD 6


bool Window::initialized = false;
UINT Window::windowCount = 0;


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
		wcex.lpszClassName = WND_CLASSNAME;
		wcex.style = CS_VREDRAW | CS_HREDRAW;
		wcex.lpfnWndProc = WndProc;

		BF(RegisterClassEx(&wcex));

		initialized = true;
	}

	RECT wr = { 0, 0, width, height };

	BF(AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE));

	handle = CreateWindow(WND_CLASSNAME, title, WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  wr.right - wr.left, wr.bottom - wr.top,
						  nullptr, nullptr, hInstance, nullptr);

	BF(handle);

	SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)this);

	ShowWindow(handle, SW_SHOW);

	windowCount++;
}

Window::~Window()
{
}

void Window::clear(float r, float g, float b, float a)
{

}

void Window::setTitle(LPCSTR title)
{
	BF(SetWindowText(handle, title));
}

void Window::setPosition(XMINT2 position)
{
	BF(SetWindowPos(handle, nullptr, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER));
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

const XMINT2& Window::getPosition()
{
	static XMINT2 position;

	RECT wr;
	BF(GetWindowRect(handle, &wr));

	position.x = wr.left;
	position.y = wr.top;

	return position;
}

const XMINT2& Window::getMousePosition()
{
	static XMINT2 mousePosition;

	POINT p;
	BF(GetCursorPos(&p));
	BF(ScreenToClient(handle, &p));

	mousePosition.x = p.x;
	mousePosition.y = p.y;

	return mousePosition;
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

HWND Window::getHandle()
{
	return handle;
}

bool Window::isKeyPressed(Key key)
{
	return keys[key];
}

bool Window::isInWindow(int x, int y, bool inClientSpace)
{
	RECT rect;

	if (inClientSpace)
		BF(GetClientRect(handle, &rect));
	else
		BF(GetWindowRect(handle, &rect));


	POINT cursorPos = { x, y };
	return PtInRect(&rect, cursorPos) > 0;
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

Window* Window::getWindow(HWND hwnd)
{
	return (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

void Window::setKeyState(Key key, bool pressed)
{
	keys[key] = pressed;
}

void Window::setMouseKeyState(RAWINPUT* ri, USHORT buttonFlagDown, USHORT buttonFlagUp, Key key)
{
	XMINT2 p = getMousePosition();

	if (isInWindow(p.x, p.y, true) && (ri->data.mouse.usButtonFlags & buttonFlagDown))
		setKeyState(key, true);
	else if (ri->data.mouse.usButtonFlags & buttonFlagUp)
		setKeyState(key, false);
}

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window *window = getWindow(hwnd);

	switch (msg)
	{
		case WM_CREATE:
		{
			const int size = 2;

			RAWINPUTDEVICE rid[size];

			rid[0].usUsagePage = 1;
			rid[0].usUsage = RID_USAGE_MOUSE;
			rid[0].dwFlags = 0;
			rid[0].hwndTarget = hwnd;

			rid[1].usUsagePage = 1;
			rid[1].usUsage = RID_USAGE_KEYBOARD;
			rid[1].dwFlags = 0;
			rid[1].hwndTarget = hwnd;

			BF(RegisterRawInputDevices(rid, size, sizeof(RAWINPUTDEVICE)));
		} break;

		case WM_INPUT:
		{
			UINT dwSize;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

			BYTE *buffer = new BYTE[dwSize];

			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)buffer, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			{
				delete buffer;
				throw std::exception("return size does not macth");
			}


			RAWINPUT *raw = (RAWINPUT*)buffer;

			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				window->setMouseKeyState(raw, RI_MOUSE_LEFT_BUTTON_DOWN, RI_MOUSE_LEFT_BUTTON_UP, Key::LButton);
				window->setMouseKeyState(raw, RI_MOUSE_RIGHT_BUTTON_DOWN, RI_MOUSE_RIGHT_BUTTON_UP, Key::RButton);
				window->setMouseKeyState(raw, RI_MOUSE_MIDDLE_BUTTON_DOWN, RI_MOUSE_MIDDLE_BUTTON_UP, Key::MButton);
				window->setMouseKeyState(raw, RI_MOUSE_BUTTON_4_DOWN, RI_MOUSE_BUTTON_4_UP, Key::XButton1);
				window->setMouseKeyState(raw, RI_MOUSE_BUTTON_5_DOWN, RI_MOUSE_BUTTON_5_UP, Key::XButton2);
			}
			else if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				USHORT keyCode = raw->data.keyboard.VKey;
				bool pressed = !(raw->data.keyboard.Flags & RI_KEY_BREAK);
				window->setKeyState((Key)keyCode, pressed);
			}

			delete buffer;
		} break;

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
				PostQuitMessage(EXIT_SUCCESS);
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