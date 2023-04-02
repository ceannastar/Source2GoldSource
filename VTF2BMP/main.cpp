
#include "vtftobmp.h"
#include <Windows.h>

VTFH* vtf = new VTFH;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM w, LPARAM l)
{
	switch (Msg)
	{
		case WM_DESTROY:
		{
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
		}
		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hWnd, Msg, w, l);
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR CmdLine, int ShowCmd)
{
	WNDCLASS wnd = {};

	if (vtf->read_vtf("brickfloor001a") == false)
		return 1;

	wnd.hInstance = hInst;
	wnd.lpfnWndProc = WindowProc;
	wnd.lpszClassName = L"Window";
	RegisterClass(&wnd);

	HWND handle_window = CreateWindow(L"Window", L"Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, nullptr, nullptr, hInst, nullptr);

	ShowWindow(handle_window, true);

	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

		}
	}

	return 0;
}