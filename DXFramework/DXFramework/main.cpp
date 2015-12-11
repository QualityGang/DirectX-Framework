#include "stdafx.h"

#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window("Title", 800, 600);

	std::string str = window.getTitle();
	str += "\n";

	OutputDebugString(str.c_str());

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (GetMessage(&msg, window.getHandle(), 0, 0))
		{
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}