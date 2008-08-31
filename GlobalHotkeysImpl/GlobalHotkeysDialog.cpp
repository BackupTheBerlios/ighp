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

#include "GlobalHotkeysDialog.h"
#include "PluginSettings.h" 

GlobalHotkeysDialog::GlobalHotkeysDialog(UINT nResID, HWND hWndParent) 
	: CDialog(nResID, hWndParent)
{
	//TODO: Add constructor code here	
}

GlobalHotkeysDialog::~GlobalHotkeysDialog()
{
	//TODO: Add destructor code here
}

BOOL GlobalHotkeysDialog::OnInitDialog()
{
	//TODO: Add init code here
	return TRUE;
}

BOOL GlobalHotkeysDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//switch (LOWORD(wParam))
    //{
	//case IDC_BUTTON1:
	//	OnButton();
	//	return TRUE;
    //} //switch (LOWORD(wParam))

	return FALSE;
}

void GlobalHotkeysDialog::OnOK()
{
	//TODO: Save settings
}