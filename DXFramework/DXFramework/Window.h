#pragma once

#include <string>

#include "Keys.h"
#include "SwapChain.h"

class Window
{
public:
	Window(LPCSTR title, int width, int height);
	//Window(HINSTANCE instance, LPCSTR title, const XMINT2 &size);

	void clear(float r, float g, float b, float a);
	const XMINT2& getMousePosition();
	void setTitle(LPCSTR title);
	void setSize(int width, int height);
	void setMinSize(int width, int height);
	void setResizable(bool resizable);
	void setMaximizable(bool maximizable);
	bool isKeyPressed(Key key);
	static Window *getWindow(HWND hwnd);
	std::string getTitle();
	const XMINT2& getSize();
	const XMINT2& getMinSize();
	bool isResizable();
	bool isMaximizable();
	HWND getHandle();
private:
	LPCSTR title;
	XMINT2 size;
	XMINT2 minSize;
	XMINT2 mousePosition;
	bool maximizable;
	bool resizable;
	bool keys[NUM_KEYCODES];
	HWND handle;
	SwapChain swapChain;

	static bool initialized;

	static LRESULT CALLBACK msgProc(HWND hwnd,
									UINT msg,
									WPARAM wParam, LPARAM lParam);
};