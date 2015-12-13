#pragma once

#include <string>

#include "Keys.h"
#include "SwapChain.h"

class Window
{
public:
	Window(LPCSTR title, int width, int height);
	virtual ~Window();

	void clear(float r, float g, float b, float a);

	void setTitle(LPCSTR title);
	void setPosition(XMINT2);
	void setSize(int width, int height);
	void setMinSize(int width, int height);
	void setResizable(bool resizable);
	void setMaximizable(bool maximizable);

	const std::string& getTitle();
	const XMINT2& getPosition();
	const XMINT2& getMousePosition();
	const XMINT2& getSize();
	const XMINT2& getMinSize();
	HWND getHandle();

	bool isKeyPressed(Key key);
	bool isResizable();
	bool isMaximizable();
	bool isInWindow(int x, int y, bool inClientSpace);

	static Window *getWindow(HWND hwnd);
private:
	HWND handle;
	XMINT2 minSize, minWndSize;
	bool keys[NUM_KEYCODES];

	SwapChain swapChain;

	static bool initialized;
	static UINT windowCount;

	void setKeyState(Key key, bool pressed);
	void setMouseKeyState(RAWINPUT *ri, USHORT buttonFlagDown, USHORT buttonFlagUp, Key key);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};