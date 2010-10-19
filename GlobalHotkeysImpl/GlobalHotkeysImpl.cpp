/*
 * Copyright (c) 2010 Stefan Cosma <stefan.cosma@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "GhPlugin.h"

#include <windows.h>

#include <QApplication>

GhPlugin *plugin;

static QApplication *application;
static bool appInitialized = false;
static HWND hwnd = NULL;

LRESULT CALLBACK WindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	return true;
}

extern "C" void WINAPI InitGlobalHotkeysPlugin()
{
	if (QApplication::instance() == NULL) {
		int argc = 1;
		application = new QApplication(argc, (char **) NULL);
		application->setQuitOnLastWindowClosed(false);
		appInitialized = true;
	}

	plugin = new GhPlugin();

	
	WNDCLASS wc;
	TCHAR szAppName [] = TEXT("GlobalHotkeysClass");

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

	hwnd = CreateWindowEx (WS_EX_TRANSPARENT, szAppName, TEXT("Global Hotkeys"),  WS_POPUP, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
}

extern "C" void WINAPI ReleaseGlobalHotkeysPlugin()
{
	if (hwnd != NULL) {
		DestroyWindow(hwnd);
		hwnd = NULL;
	}

	if (plugin != NULL) {
		delete plugin;
		plugin = NULL;
	}

	if (appInitialized && application != NULL) {
		delete application;
		application = NULL;
	}
}

extern "C" void WINAPI Initialize()
{
	// Nothing to do
}

extern "C" void WINAPI Release()
{
	ReleaseGlobalHotkeysPlugin();
}

LRESULT CALLBACK WindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
			plugin->setHWnd(hwnd);
			plugin->loadHotkeys();
			plugin->registerHotkeys();
            break;
        case WM_HOTKEY:
            plugin->handleGlobalKey(wParam);
            break;
        case WM_DESTROY:
            plugin->unregisterHotkeys();
            return 0;
    }

    return DefWindowProc (hwnd, message, wParam, lParam);
}