// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Plugin.h>

LRESULT CALLBACK WindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
    {
        case WM_CREATE:
		{
            break;
		}

        case WM_DESTROY:
		{
            return 0;
		}
    }

    return DefWindowProc (hwnd, message, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdLine, int cmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    UNREFERENCED_PARAMETER(cmdShow);

	HWND hWnd;
	MSG msg;
	HACCEL hAccelTable;

	InitPlugin();

	WNDCLASS wc;
	TCHAR szAppName [] = TEXT("GlobalHotkeysPluginTests");

	wc.style = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppName;

	RegisterClass (&wc);

	hWnd = CreateWindowEx (WS_EX_TRANSPARENT, szAppName, TEXT("Global Hotkeys Plugin Tests"),  WS_POPUP, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);

	UpdateWindow(hWnd);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(109));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	ReleasePlugin();

	return (int) msg.wParam;
}

