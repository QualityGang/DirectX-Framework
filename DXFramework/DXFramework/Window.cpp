#include "stdafx.h"
#include "Window.h"

#include "OutputEnum.h"


#define IsMenuActiveByAlt(lParam) ((lParam >> 16) <= 0)

#define WND_CLASSNAME "DXFrameworkClassName"

#define RID_USAGE_MOUSE 2
#define RID_USAGE_KEYBOARD 6


Window::StaticInit Window::__static_init;

UINT Window::windowCount = 0;


Window::Window(LPCSTR title, int width, int height)
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(nullptr);

	int clientWidth = width;
	int clientHeight = height;
	clientToScreen(&width, &height, WS_OVERLAPPEDWINDOW);

	handle = CreateWindow(WND_CLASSNAME, title, WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT, CW_USEDEFAULT, width, height,
						  nullptr, nullptr, hInstance, nullptr);

	SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow(handle, SW_SHOW);
	init(handle, (float)clientWidth, (float)clientHeight);

	windowCount++;
}

Window::~Window()
{
	if (isFullscreen())
		setFullscreen(false);

	handle = nullptr;
}

Window::StaticInit::StaticInit()
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.hInstance = (HINSTANCE)GetModuleHandle(nullptr);
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszClassName = WND_CLASSNAME;
	//wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc = WndProc;

	RegisterClassEx(&wcex);
}

Window::StaticInit::~StaticInit()
{
}

void Window::setTitle(LPCSTR title)
{
	SetWindowText(handle, title);
}

void Window::setPosition(XMINT2 position)
{
	SetWindowPos(handle, nullptr, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Window::setSize(int width, int height)
{
	clientToScreen(&width, &height);
	SetWindowPos(handle, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void Window::setMinSize(int width, int height)
{
	minSize.x = width;
	minSize.y = height;

	clientToScreen(&width, &height);

	minWndSize.x = width;
	minWndSize.y = height;
}

void Window::setResizable(bool resizable)
{
	LONG style = GetWindowLong(handle, GWL_STYLE);

	if (resizable)
		style |= WS_THICKFRAME;
	else
		style &= ~WS_THICKFRAME;

	SetWindowLong(handle, GWL_STYLE, style);
}

void Window::setMaximizable(bool maximizable)
{
	LONG style = GetWindowLong(handle, GWL_STYLE);

	if (maximizable)
		style |= WS_MAXIMIZEBOX;
	else
		style &= ~WS_MAXIMIZEBOX;

	SetWindowLong(handle, GWL_STYLE, style);
}

void Window::setFullscreen(bool fullscreen)
{
	if (fullscreen == isFullscreen())
		return;

	OutputEnum outputEnum;
	const OutputEnum::Output &output = outputEnum.getOutput(handle);

	if (fullscreen)
	{
		DXGI_MODE_DESC modeDesc;
		ZeroMemory(&modeDesc, sizeof(DXGI_MODE_DESC));
		modeDesc.Width = output.desktopArea.right - output.desktopArea.left;
		modeDesc.Height = output.desktopArea.bottom - output.desktopArea.top;

		HRESULT hr = swapChain->ResizeTarget(&modeDesc);

		if (FAILED(hr))
			PrintError(hr);

		hr = swapChain->SetFullscreenState(true, output.dxgiOutput);

		if (FAILED(hr))
			PrintError(hr);
	}
	else
	{
		HRESULT hr = swapChain->SetFullscreenState(false, nullptr);

		if (FAILED(hr))
			PrintError(hr);

		DXGI_MODE_DESC modeDesc;
		ZeroMemory(&modeDesc, sizeof(DXGI_MODE_DESC));
		modeDesc.Width = 800;
		modeDesc.Height = 600;

		hr = swapChain->ResizeTarget(&modeDesc);

		if (FAILED(hr))
			PrintError(hr);
	}
}

void Window::getTitle(std::string *title)
{
	int len = GetWindowTextLength(handle) + 1;
	std::vector<char> buf(len);
	GetWindowText(handle, &buf[0], len);
	*title = &buf[0];
}

void Window::getPosition(XMINT2 *position)
{
	RECT wr;
	GetWindowRect(handle, &wr);

	position->x = wr.left;
	position->y = wr.top;
}

void Window::getMousePosition(XMINT2 *mousePosition)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(handle, &p);

	mousePosition->x = p.x;
	mousePosition->y = p.y;
}

void Window::getSize(XMINT2 *size)
{
	RECT cr;
	GetClientRect(handle, &cr);

	size->x = cr.right - cr.left;
	size->y = cr.bottom - cr.top;
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
		GetClientRect(handle, &rect);
	else
		GetWindowRect(handle, &rect);

	POINT cursorPos = { x, y };
	return PtInRect(&rect, cursorPos) > 0;
}

void Window::addOnResizeListener(OnResizeListener listener)
{
	onResizeListeners.push_back(listener);
}

void Window::removeOnResizeListener(int index)
{
	onResizeListeners.erase(onResizeListeners.begin() + index);
}

void Window::close()
{
	SendMessage(handle, WM_CLOSE, (WPARAM)0, (LPARAM)0);
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

bool Window::isMinimized()
{
	return IsIconic(handle) > 0;
}

bool Window::isFullscreen()
{
	BOOL fullscreen = FALSE;
	HRESULT hr = swapChain->GetFullscreenState(&fullscreen, nullptr);

	if (FAILED(hr))
		PrintError(hr);

	return fullscreen > 0;
}

Window* Window::GetWindow(HWND hwnd)
{
	return (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

void Window::clientToScreen(int *width, int *height)
{
	LONG style = GetWindowLong(handle, GWL_STYLE);
	clientToScreen(width, height, style);
}

void Window::clientToScreen(int *width, int *height, LONG style)
{
	RECT wr = { 0, 0, *width, *height };
	AdjustWindowRect(&wr, style, FALSE);

	*width = wr.right - wr.left;
	*height = wr.bottom - wr.top;
}

void Window::setKeyState(Key key, bool pressed)
{
	keys[key] = pressed;
}

void Window::setMouseKeyState(RAWINPUT* ri, USHORT buttonFlagDown, USHORT buttonFlagUp, Key key)
{
	XMINT2 p;
	getMousePosition(&p);

	if (isInWindow(p.x, p.y, true) && (ri->data.mouse.usButtonFlags & buttonFlagDown))
		setKeyState(key, true);
	else if (ri->data.mouse.usButtonFlags & buttonFlagUp)
		setKeyState(key, false);
}

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window *window = GetWindow(hwnd);

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

			RegisterRawInputDevices(rid, size, sizeof(RAWINPUTDEVICE));
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
				return DefWindowProc(hwnd, msg, wParam, lParam);

			LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
			mmi->ptMinTrackSize.x = window->minWndSize.x;
			mmi->ptMinTrackSize.y = window->minWndSize.y;
		} break;

		case WM_SIZE:
		{
			int width = (int)(short)LOWORD(lParam);
			int height = (int)(short)HIWORD(lParam);

			for (auto &listener : window->onResizeListeners)
				listener(window, width, height);

		} break;

		case WM_DESTROY:
			window->handle = nullptr;

			if (--windowCount == 0)
				PostQuitMessage(EXIT_SUCCESS);

			break;

		case WM_SYSCOMMAND:
			switch (wParam)
			{
				case SC_KEYMENU:
					if (IsMenuActiveByAlt(lParam))
						return 0;
					break;
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}