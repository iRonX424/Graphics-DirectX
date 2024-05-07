#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE prevhInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//creating a window class
	const LPCWSTR windowClassName = L"3D Window Class";
	WNDCLASSEX wndClass = {0};
	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpszClassName = windowClassName;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.style = CS_OWNDC;
	
	//optional to define
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = nullptr;
	wndClass.hIcon = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.lpszMenuName = nullptr;

	RegisterClassEx(&wndClass);

	//creating a window instance
	HWND hWnd = CreateWindowEx(0, windowClassName, L"3D Graphics Window", 
								WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
								240, 240, 640, 480, nullptr, nullptr, hInstance, nullptr);

	//show the created window
	ShowWindow(hWnd, SW_SHOW);

	//iterate through window messages
	MSG msg;
	BOOL bMsgReturnVal;

	while ((bMsgReturnVal = GetMessage(&msg, nullptr, 0, 0)) != 0)
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}

	if (bMsgReturnVal == -1)
		return -1;
	else
		return msg.wParam;
}