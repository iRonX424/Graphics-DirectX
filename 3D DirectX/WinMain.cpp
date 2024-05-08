#include "Window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE prevhInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window(800, 300, "Graphics Practice Window");

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1)
	{
		return -1;
	}

	return msg.wParam;
}