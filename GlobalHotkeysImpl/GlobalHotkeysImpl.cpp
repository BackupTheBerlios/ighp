/*
 * Copyright (c) 2008 Stefan Cosma <stefan.cosma@gmail.com>
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

#include <windows.h>

#include "iTunesCOMInterface.h"
#include "PluginSettings.h" 

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

void RegisterGlobalKeys(HWND hwnd)
{
	std::map<const unsigned int, Hotkey*>* hotkeys = PluginSettings::Instance()->GetHotkeys();
	std::map<const unsigned int, Hotkey*>::iterator iter;

	for (iter = hotkeys->begin(); iter != hotkeys->end(); iter++) {
		unsigned int modifiers = (iter->second->GetAlt() ? MOD_ALT : 0) | 
			                     (iter->second->GetControl() ? MOD_CONTROL : 0) | 
			                     (iter->second->GetShift() ? MOD_SHIFT : 0) | 
			                     (iter->second->GetWin() ? MOD_WIN : 0);
		RegisterHotKey(hwnd, iter->first, modifiers, iter->second->GetKeyCode());
	}
}

void UnregisterGlobalKeys(HWND hwnd)
{
	std::map<const unsigned int, Hotkey*>* hotkeys = PluginSettings::Instance()->GetHotkeys();
	std::map<const unsigned int, Hotkey*>::iterator iter;

	for (iter = hotkeys->begin(); iter != hotkeys->end(); iter++) {
		UnregisterHotKey(hwnd, iter->first);
		delete iter->second;
		iter->second = 0;
	}
}

void HandleGlobalKey(WPARAM wParam, LPARAM lParam)
{
	std::map<const unsigned int, Hotkey*>& hotkeys = *PluginSettings::Instance()->GetHotkeys();
	hotkeys[wParam]->PerformAction();
}

extern "C" void WINAPI Initialize()
{
	// Write here all the code you need to initialize the DLL
}

extern "C" void WINAPI Release()
{
	// Write here all the code you need to free everything ...
}

extern "C" void WINAPI InitGlobalHotkeysPlugin()
{
	HWND hwnd;
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
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL) ; 
}

LRESULT CALLBACK WindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			PluginSettings::Instance()->ReadConfigFile();
			RegisterGlobalKeys(hwnd);
			break;
		case WM_HOTKEY:
			HandleGlobalKey(wParam, lParam);
			break;
		case WM_DESTROY:
			UnregisterGlobalKeys(hwnd);
			PluginSettings::Destroy();
			PostQuitMessage (0);
			return 0;
	}

	return DefWindowProc (hwnd, message, wParam, lParam);
}