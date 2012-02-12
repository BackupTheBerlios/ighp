// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Plugin.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    UNREFERENCED_PARAMETER(cmdShow);

	InitPlugin();

	MessageBox(NULL, TEXT("Close dialog to end tests"), TEXT("Global Hotkeys Test"),
		MB_OK | MB_ICONEXCLAMATION | MB_SETFOREGROUND);

	ReleasePlugin();

	return 0;
}

