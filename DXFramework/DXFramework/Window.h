#pragma once

#include <Windows.h>
#include <DirectXMath.h>

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
	//bool keys[NUM_KEYCODES];
	HWND handle_;
	//SwapChain swap_chain_;

	static LRESULT msgProc(HWND hwnd,
		UINT msg,
		WPARAM wParam, LPARAM lParam);
public:
	Window(LPCSTR title, int width, int height);
	Window(LPCSTR title, const XMINT2 &size);
	void clear(float r, float g, float b, float a);
	void getMousePosition(POINT *p);
	void setTitle(LPCSTR title);
	void setSize(int width, int height);
	void setMinSize(int width, int height);
	void setResizable(bool resizable);
	void setMaximizable(bool maximizable);
	//bool isKeyPressed(Key key);
	LPCSTR getTitle();
	XMINT2 getSize();
	XMINT2 getMinSize();
	bool isResizable();
	bool isMaximizable();
	HWND getHandle();

};