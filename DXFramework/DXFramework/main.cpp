#include "stdafx.h"

#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window("Title", 800, 600);

	std::string str = window.getTitle();
	str += "\n";

	OutputDebugString(str.c_str());

	return 0;
}