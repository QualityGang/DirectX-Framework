#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <iostream>
#include "Keys.h"
#include "SwapChain.h"

using DirectX::XMINT2;

class Window
{
private:
	LPCSTR title_;
	XMINT2 size_;
	XMINT2 min_size_;
	XMINT2 mouse_position_;
	bool maximizable_;
	bool resizable_;
	bool keys[NUM_KEYCODES];
	HWND handle_;
	SwapChain swap_chain_;

	static LRESULT CALLBACK msgProc(HWND hwnd,
		UINT msg,
		WPARAM wParam, LPARAM lParam);
public:
	Window(HINSTANCE instance, LPCSTR title, int width, int height);
	Window(HINSTANCE instance, LPCSTR title, const XMINT2 &size);
	void clear(float r, float g, float b, float a);
	void getMousePosition(POINT *p);
	void setTitle(LPCSTR title);
	void setSize(int width, int height);
	void setMinSize(int width, int height);
	void setResizable(bool resizable);
	void setMaximizable(bool maximizable);
	bool isKeyPressed(Key key);
	static Window* getWindowLong(HWND hwnd);
	LPCSTR getTitle();
	XMINT2 getSize();
	XMINT2 getMinSize();
	bool isResizable();
	bool isMaximizable();
	HWND getHandle();
};