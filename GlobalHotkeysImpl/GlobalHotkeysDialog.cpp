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

#include <commctrl.h>

extern std::map<const std::string, Actions> actionsMap;

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
	InitHotkeysListViewColumns();
	PopulateHotkeysList();

	PopulateActionsComboBox();

	return TRUE;
}

BOOL GlobalHotkeysDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
    {
	case IDAPPLY:
		OnApply();
		return TRUE;
    }

	return FALSE;
}

void GlobalHotkeysDialog::OnOK()
{
	OnApply();
}

void GlobalHotkeysDialog::OnApply()
{

}

void GlobalHotkeysDialog::InitHotkeysListViewColumns()
{
	HWND hwndListView = GetDlgItem(IDC_HOTKEYS_LIST);

	ListView_SetExtendedListViewStyle(hwndListView, LVS_EX_FULLROWSELECT);

	// init columns
	LVCOLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_ORDER | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 172;

	// Action
	lvc.iOrder = 0;
	lvc.iSubItem = 0;
	lvc.pszText = TEXT("Action");

	ListView_InsertColumn(hwndListView, 0, &lvc);

	// Hotkey
	lvc.iOrder = 1;
	lvc.iSubItem = 1;
	lvc.pszText = TEXT("Hotkey");

	ListView_InsertColumn(hwndListView, 1, &lvc);
}

void GlobalHotkeysDialog::AddHotkeyListItem(const std::string action, const std::string hotkey)
{
	static int index = 0;
	HWND hwndListView = GetDlgItem(IDC_HOTKEYS_LIST);

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.state = 0;
	lvi.stateMask = 0;
	lvi.iItem = index++;
	lvi.iSubItem = 0;
	lvi.lParam = (LPARAM) action.c_str();
	lvi.pszText = (LPSTR) action.c_str();

	ListView_InsertItem(hwndListView, &lvi);

	lvi.iSubItem = 1;
	lvi.lParam = (LPARAM) hotkey.c_str();
	lvi.pszText = (LPSTR) hotkey.c_str();

	SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM) &lvi); 
}

void GlobalHotkeysDialog::PopulateHotkeysList()
{
	HWND hwndListView = GetDlgItem(IDC_HOTKEYS_LIST);

	std::map<const unsigned int, Hotkey*>* hotkeys = PluginSettings::Instance()->GetHotkeys();
	std::map<const unsigned int, Hotkey*>::iterator iter;

	for (iter = hotkeys->begin(); iter != hotkeys->end(); iter++) {
		const std::string action = iter->second->GetActionName();
		const std::string hotkey = iter->second->GetHotkeyName();
		
		AddHotkeyListItem(action, hotkey);
	}
}

void GlobalHotkeysDialog::PopulateActionsComboBox()
{
	HWND hwndCombo = GetDlgItem(IDC_ACTIONS_COMBO);

	std::map<const std::string, Actions>::iterator iter;
	for (iter = actionsMap.begin(); iter != actionsMap.end(); iter++) {
		SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM) iter->first.c_str());
	}	 
}