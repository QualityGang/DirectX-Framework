#pragma once

#include <functional>
#include <vector>
#include <string>

#include "D3D11Renderer.h"
#include "RenderTarget.h"
#include "NonCopyable.h"
#include "Keys.h"

class Window : public RenderTarget
{
public:
	Window(LPCSTR title, int width, int height);
	virtual ~Window();

	void setTitle(LPCSTR title);
	void setPosition(XMINT2);
	void setSize(int width, int height);
	void setMinSize(int width, int height);
	void setResizable(bool resizable);
	void setMaximizable(bool maximizable);
	void setFullscreen(bool fullscreen);

	void getTitle(std::string *title);
	void getPosition(XMINT2 *position);
	void getMousePosition(XMINT2 *mousePosition);
	void getSize(XMINT2 *size);
	const XMINT2& getMinSize();
	HWND getHandle();

	bool isKeyPressed(Key key);
	bool isResizable();
	bool isMaximizable();
	bool isMinimized();
	bool isFullscreen();
	bool isInWindow(int x, int y, bool inClientSpace);

	typedef std::function<void(Window *window, int width, int height)> OnResizeListener;
	void addOnResizeListener(OnResizeListener listener);
	void removeOnResizeListener(int index);

	void resize(int width, int height);
	void present();
	void close();

	static Window* GetWindow(HWND hwnd);
private:
	IDXGISwapChain *swapChain;
	HWND handle;
	XMINT2 minSize, minWndSize;
	bool keys[NUM_KEYCODES];

	std::vector<OnResizeListener> onResizeListeners;

	static UINT windowCount;

	void clientToScreen(int *width, int *height);
	void clientToScreen(int *width, int *height, LONG style);

	void setKeyState(Key key, bool pressed);
	void setMouseKeyState(RAWINPUT *ri, USHORT buttonFlagDown, USHORT buttonFlagUp, Key key);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	class StaticInit
	{
	public:
		StaticInit();
		virtual ~StaticInit();
	};

	static StaticInit __static_init;
};