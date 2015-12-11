#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <iostream>
#include "Keys.h"
#include "SwapChain.h"

using DirectX::XMINT2;
#define IsMenuActiveByAlt (lParam >> 16) <= 0 

class Window
{
public:
	Window(HINSTANCE instance, LPCSTR title, int width, int height);
	//Window(HINSTANCE instance, LPCSTR title, const XMINT2 &size);

	void clear(float r, float g, float b, float a);
	XMINT2 getMousePosition();
	void setTitle(LPCSTR title);
	void setSize(int width, int height);
	void setMinSize(int width, int height);
	void setResizable(bool resizable);
	void setMaximizable(bool maximizable);
	bool isKeyPressed(Key key);
	static Window *getWindow(HWND hwnd);
	LPCSTR getTitle();
	XMINT2& getSize();
	XMINT2& getMinSize();
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

	static LRESULT CALLBACK msgProc(HWND hwnd,
									UINT msg,
									WPARAM wParam, LPARAM lParam);
};