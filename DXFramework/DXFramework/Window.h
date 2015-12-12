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
	void setPosition(XMINT2);
	bool isKeyPressed(Key key);
	static Window *getWindow(HWND hwnd);
	std::string getTitle();
	XMINT2 getSize();
	const XMINT2& getMinSize();
	const XMINT2& getPosition();
	bool isResizable();
	bool isMaximizable();
	HWND getHandle();
private:
	XMINT2 minSize;
	XMINT2 mousePosition;
	bool keys[NUM_KEYCODES];
	HWND handle;
	SwapChain swapChain;

	static bool initialized;
	static UINT windowCount;

	static LRESULT CALLBACK msgProc(HWND hwnd,
									UINT msg,
									WPARAM wParam, LPARAM lParam);
};