#include "Window.h"

//access to window class
Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept
	:hInstance(GetModuleHandle(nullptr))
{
	//create window class
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = HandleMessageSetup;
	wndClass.hInstance = GetInstance();
	wndClass.lpszClassName = GetName();

	//optional to define
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = nullptr;
	wndClass.hIcon = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.lpszMenuName = nullptr;

	RegisterClassEx(&wndClass);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(GetName(), GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return windowClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return windowClass.hInstance;
}

Window::Window(int width, int height, const char* name) noexcept
{
	RECT actualWindow;
	actualWindow.left = 100;
	actualWindow.right = width + actualWindow.left;
	actualWindow.top = 100;
	actualWindow.bottom = height + actualWindow.top;
	AdjustWindowRect(&actualWindow, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	//create window
	//CreateWindow - might have problems
	hWnd = CreateWindow(WindowClass::GetName(), name, 
							WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 
								CW_USEDEFAULT, CW_USEDEFAULT, actualWindow.right - actualWindow.left, actualWindow.bottom - actualWindow.top,
									nullptr, nullptr, WindowClass::GetInstance(), this);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		//extract ptr to window class from the creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		//set WinAPI managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageThought));

		//forward message to window class handler
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}
	//if we get a message before the WM_NCCREATE, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessageThought(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//retrieve ptr to window class
	Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return pWindow->HandleMessage(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}