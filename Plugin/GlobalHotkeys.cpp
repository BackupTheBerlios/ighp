/*
 * Copyright (c) 2008-2012 Stefan Cosma <stefan.cosma@gmail.com>
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalHotkeys.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT GlobalHotkeys::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = GetHwnd();

	switch (uMsg)
	{
		case WM_CREATE:
		{
			OnCreate(hWnd);
			break;
		}
		
		case WM_HOTKEY:
		{
			OnHotkey(wParam, lParam);
			break;
		}
		
		case WM_DESTROY:
		{
			OnDestroy(hWnd);
			break;
		}
	}

	// Pass unhandled messages on to parent DialogProc
	return WndProcDefault(uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GlobalHotkeys::PreRegisterClass(WNDCLASS &wc)
{
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= CWnd::StaticWindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= GetApp()->GetInstanceHandle();
	wc.hIcon			= NULL;
	wc.hCursor			= LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= TEXT("GlobalHotkeysPlugin");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GlobalHotkeys::PreCreate(CREATESTRUCT &cs)
{
	cs.cx             = CW_USEDEFAULT;
	cs.cy             = CW_USEDEFAULT;
	cs.dwExStyle      = WS_EX_TRANSPARENT;
	cs.hInstance      = GetApp()->GetInstanceHandle();
	cs.hMenu          = NULL;
	cs.hwndParent     = NULL;
	cs.lpCreateParams = cs.lpCreateParams;
	cs.lpszClass      = cs.lpszClass;
	cs.lpszName       = TEXT("Global Hotkeys Plugin");
	cs.style          = WS_POPUP;
	cs.x              = CW_USEDEFAULT;
	cs.y              = CW_USEDEFAULT;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GlobalHotkeys::OnCreate(HWND hWnd)
{
	LoadHotkeys();

	for (HotKeysIterator it = m_hotkeys.begin(); it != m_hotkeys.end(); ++it)
	{
		HotKey hk = (*it);

		unsigned int modifiers = 0;
		if (hk.keycomb.alt)		modifiers |= MOD_ALT;
		if (hk.keycomb.control)	modifiers |= MOD_CONTROL;
		if (hk.keycomb.shift)	modifiers |= MOD_SHIFT;
		if (hk.keycomb.meta)	modifiers |= MOD_WIN;

		if (RegisterHotKey(hWnd, hk.command, modifiers, hk.keycomb.key) == FALSE)
		{
			// TODO: Report error
			OutputDebugString(TEXT("Error while registering hotkey\n"));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GlobalHotkeys::OnHotkey(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	if (wParam < 0 || wParam >= CMD_COUNT)
	{
		// TODO: Report error
		OutputDebugString(TEXT("Error while handling hotkey\n"));
		return;
	}

	COMMANDS[wParam].execute();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GlobalHotkeys::OnDestroy(HWND hWnd)
{
	for (HotKeysIterator it = m_hotkeys.begin(); it != m_hotkeys.end(); ++it)
	{
		HotKey hk = (*it);

		if (UnregisterHotKey(hWnd, hk.command) == FALSE)
		{
			// TODO: Report error
			OutputDebugString(TEXT("Error while unregistering hotkey\n"));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern HANDLE g_dllHandle;

PluginWinApp::PluginWinApp()
	: m_pMainWindow(0)
{
	SetResourceHandle((HINSTANCE) g_dllHandle);

	m_pMainWindow = new GlobalHotkeys();
	m_pMainWindow->Create();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PluginWinApp::~PluginWinApp()
{
	m_pMainWindow->Destroy();
	//::PostQuitMessage(0); // really ?!?!

	SAFE_DEL(m_pMainWindow);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
