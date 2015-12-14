#include "stdafx.h"

#include "Window.h"
#include "Pipeline.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window("Title", 800, 600);
	window.setMaximizable(false);
	window.setResizable(false);

	std::string str = window.getTitle();
	str += "\n";

	OutputDebugString(str.c_str());

	MSG msg = { 0 };
	Pipeline::setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, window.getHandle(), 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}
		else
		{
			bool b = window.isKeyPressed(Key::A);
			std::string s = std::to_string(b);
			s += "\n";
			OutputDebugString(s.c_str());

			Sleep(100);
		}
	}

	return (int)msg.wParam;
}