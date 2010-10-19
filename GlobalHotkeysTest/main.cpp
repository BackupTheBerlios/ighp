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

#include <QApplication>
#include <QDialog>
#include <GlobalHotkeysImpl.h>
#include <stdlib.h>
#include <shlobj.h>

static HMODULE hDLL;
static DLL_Function_InitGlobalHotkeysPlugin InitGlobalHotkeysPlugin;
static DLL_Function_ReleaseGlobalHotkeysPlugin ReleaseGlobalHotkeysPlugin;

#define kGlobalHotkeysImpl              

static void LoadGlobalHotkeysImplDll()
{
	if (NULL == (hDLL = LoadLibrary(TEXT("GlobalHotkeysImpl.dll"))))
		MessageBox(NULL, TEXT("Could nod load GlobalHotkeysImpl.dll"), TEXT("Global Hotkeys"), MB_OK | MB_ICONERROR);

	InitGlobalHotkeysPlugin = (DLL_Function_InitGlobalHotkeysPlugin) GetProcAddress(hDLL, "InitGlobalHotkeysPlugin");
	ReleaseGlobalHotkeysPlugin = (DLL_Function_ReleaseGlobalHotkeysPlugin) GetProcAddress(hDLL, "ReleaseGlobalHotkeysPlugin");
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);

	LoadGlobalHotkeysImplDll();
	InitGlobalHotkeysPlugin();

	return app.exec();
	return 0;
}
