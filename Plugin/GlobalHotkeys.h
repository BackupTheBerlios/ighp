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

#pragma once

#include <boost/utility.hpp>

#include "Singleton.h"
#include "HotKeyManager.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <wincore.h>

class GlobalHotkeys : public CWnd, public HotKeyManager, boost::noncopyable
{
public:
	GlobalHotkeys()				{ }
	virtual ~GlobalHotkeys()	{ }

	void RegisterHotkeys();
	void UnregisterHotkeys();
	void ReloadHotkeys();

private:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void PreRegisterClass(WNDCLASS &wc);
	virtual void PreCreate(CREATESTRUCT &cs);

	void OnCreate(HWND hWnd);
	void OnHotkey(WPARAM wParam, LPARAM lParam);
	void OnDestroy(HWND hWnd);
};

class PluginWinApp : public CWinApp, boost::noncopyable
{
public:
	PluginWinApp();
	virtual ~PluginWinApp();

	virtual BOOL InitInstance();

	GlobalHotkeys* GetMainWindow() { return &m_mainWindow; }
	void Quit() { ::PostQuitMessage(0); }

private:
	GlobalHotkeys m_mainWindow;
};

typedef Singleton<PluginWinApp> Plugin;