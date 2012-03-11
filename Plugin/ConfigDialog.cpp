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
#include "ConfigDialog.h"
#include "Plugin.h"
#include "GlobalHotkeys.h"

#include "commands.h"

#include <string>
#include <map>


typedef std::map<unsigned int, const string_t>				KeyNames;
typedef std::map<unsigned int, const string_t>::iterator	KeyNamesIterator;

KeyNames g_KeyNames;

void InitKeyNames();
string_t BuildKeyCombinationString(KeyCombination kc);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ConfigDialog::OnInitDialog()
{
	SetIconLarge(IDI_HOTKEYS_ICON);
	SetIconSmall(IDI_HOTKEYS_ICON);

	if (g_KeyNames.size() == 0)
	{
		::InitKeyNames();
	}

	m_hotkeys.clear();
	LoadHotkeys();

	m_listView.AttachDlgItem(IDC_HOTKEYS_LIST, this);
	m_listView.SetHotkeysList(&m_hotkeys);
	m_listView.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listView.PopulateList();

	m_comboBox.AttachDlgItem(IDC_ACTIONS_COMBO, this);
	for (unsigned int i = 0; i < CMD_COUNT; ++i)
	{
		TCHAR text[256] = {0};

		::LoadString(GetApp()->GetInstanceHandle(), COMMANDS[i].textid,
				text, sizeof(text)/sizeof(text[0]));

		int result = m_comboBox.AddString(text);
		result++;
	}
	m_comboBox.SetCurSel(0);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ConfigDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
    {
		case IDC_APPLY:
		{
			OnApply();
			return TRUE;
		}

		case IDC_ADD:
		{
			OnAdd();
			return TRUE;
		}

		case IDC_CLEAR:
		{
			OnRemove();
			return TRUE;
		}

		case IDC_MODIFY:
		{
			OnModify();
			return TRUE;
		}
    }

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConfigDialog::OnOK()
{
	OnApply();
	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConfigDialog::OnApply()
{
	SaveHotkeys();
	Plugin::GetInstance()->GetMainWindow()->ReloadHotkeys();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConfigDialog::OnAdd()
{
	eCommand command = static_cast<eCommand>(m_comboBox.GetCurSel());
	HotkeyDialog hkDialog(command, this);

	int result = hkDialog.DoModal();

	if (result != IDOK)
	{
		return;
	}

	HotKey hk = hkDialog.GetHotKey();

	if (IsDefined(hk))
	{
		TCHAR text[256] = {0};

		::LoadString(GetApp()->GetInstanceHandle(), IDS_DUPLICATE_ERROR_TEXT,
				text, sizeof(text)/sizeof(text[0]));

		TCHAR caption[256] = {0};

		::LoadString(GetApp()->GetInstanceHandle(), IDS_DUPLICATE_ERROR_CAPTION,
				caption, sizeof(caption)/sizeof(caption[0]));

		MessageBox(text, caption, MB_OK | MB_ICONERROR | MB_APPLMODAL);

		return;
	}

	m_hotkeys.push_back(hk);
	m_listView.InsertHotkey(hk);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConfigDialog::OnRemove()
{
	int itemIndex = m_listView.GetSelectedItem();

	LVITEM lvItem;
	ZeroMemory(&lvItem, sizeof(LVITEM));
	lvItem.iItem	= itemIndex;
	lvItem.mask		= LVIF_PARAM;

	if (m_listView.GetItem(lvItem))
	{
		int index = lvItem.lParam;
		m_hotkeys.erase(m_hotkeys.begin() + index);
	}

	m_listView.PopulateList();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConfigDialog::OnModify()
{
	int itemIndex = m_listView.GetSelectedItem();

	LVITEM lvItem;
	ZeroMemory(&lvItem, sizeof(LVITEM));
	lvItem.iItem	= itemIndex;
	lvItem.mask		= LVIF_PARAM;

	if (!m_listView.GetItem(lvItem))
	{
		return;
	}

	int index = lvItem.lParam;

	HotkeyDialog hkDialog(m_hotkeys[index].command, this);

	int result = hkDialog.DoModal();

	if (result != IDOK)
	{
		return;
	}

	HotKey hk = hkDialog.GetHotKey();

	if (IsDefined(hk))
	{
		TCHAR text[256] = {0};

		::LoadString(GetApp()->GetInstanceHandle(), IDS_DUPLICATE_ERROR_TEXT,
				text, sizeof(text)/sizeof(text[0]));

		TCHAR caption[256] = {0};

		::LoadString(GetApp()->GetInstanceHandle(), IDS_DUPLICATE_ERROR_CAPTION,
				caption, sizeof(caption)/sizeof(caption[0]));

		MessageBox(text, caption, MB_OK | MB_ICONERROR | MB_APPLMODAL);

		return;
	}

	m_hotkeys[index].keycomb = hk.keycomb;
	m_listView.UpdateHotkey(itemIndex, hk.keycomb);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HotkeysListView::OnInitialUpdate()
{
	LVCOLUMN lvc;
	ZeroMemory(&lvc, sizeof(LVCOLUMN));

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 172;

	TCHAR text[256] = {0};

	::LoadString(GetApp()->GetInstanceHandle(), IDS_COLUMN_COMMAND,
			text, sizeof(text)/sizeof(text[0]));

	lvc.iSubItem = 0;
	lvc.pszText = text;

	InsertColumn(0, lvc);

	::LoadString(GetApp()->GetInstanceHandle(), IDS_COLUMN_KEYCOMB,
			text, sizeof(text)/sizeof(text[0]));

	lvc.iSubItem = 1;
	lvc.pszText = text;

	InsertColumn(1, lvc);

	DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
	SetWindowLongPtr(GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HotkeysListView::PopulateList()
{
	DeleteAllItems();
	UpdateGuiFromSelection();

	for (size_t i = 0; i < m_pHotkeys->size(); ++i)
	{
		InsertHotkey(m_pHotkeys->at(i));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HotkeysListView::InsertHotkey(HotKey hk)
{
	TCHAR text[256] = {0};

	::LoadString(GetApp()->GetInstanceHandle(), COMMANDS[hk.command].textid,
			text, sizeof(text)/sizeof(text[0]));

	LVITEM lvItem;
	ZeroMemory(&lvItem, sizeof(LVITEM));

	lvItem.mask		= LVIF_PARAM | LVIF_TEXT;
	lvItem.pszText	= (LPTSTR) text;
	lvItem.lParam	= (LPARAM) GetItemCount();

	int item = InsertItem(lvItem);
		
	const string_t str = BuildKeyCombinationString(hk.keycomb);
	ZeroMemory(&lvItem, sizeof(LVITEM));

	lvItem.mask		= LVIF_TEXT;
	lvItem.iItem	= item;
	lvItem.iSubItem	= 1;
	lvItem.pszText	= (LPTSTR) str.c_str();

	SendMessage(LVM_SETITEM, 0, (LPARAM)&lvItem);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HotkeysListView::UpdateHotkey(int item, KeyCombination keycomb)
{
	LVITEM lvItem;
	ZeroMemory(&lvItem, sizeof(LVITEM));

	const string_t str = BuildKeyCombinationString(keycomb);

	lvItem.mask		= LVIF_TEXT;
	lvItem.iItem	= item;
	lvItem.iSubItem	= 1;
	lvItem.pszText	= (LPTSTR) str.c_str();

	SendMessage(LVM_SETITEM, 0, (LPARAM)&lvItem);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HotkeysListView::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_SHOWSELALWAYS | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_ALIGNLEFT | LVS_NOSORTHEADER;
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT HotkeysListView::OnNotifyReflect(WPARAM wParam, LPARAM lParam)
{
	LPNMHDR  pnmh = (LPNMHDR) lParam;

	switch(pnmh->code)
	{
		case NM_CLICK:
		{
			UpdateGuiFromSelection();
			break;
		}

		case NM_DBLCLK:
		{
			UpdateGuiFromSelection();

			WPARAM wParam = MAKEWPARAM(IDC_MODIFY, BN_CLICKED);
			LPARAM lParam = (LPARAM) GetParent()->GetDlgItem(IDC_MODIFY);
			SendMessage(GetParent()->GetHwnd(), WM_COMMAND, wParam, lParam);
			break;
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HotkeysListView::UpdateGuiFromSelection()
{
	ConfigDialog* pDialog = (ConfigDialog*) GetParent();

	int itemIndex = GetSelectedItem();

	LVITEM lvItem;
	ZeroMemory(&lvItem, sizeof(LVITEM));
	lvItem.iItem	= itemIndex;
	lvItem.mask		= LVIF_PARAM;

	int index = -1;
	if (GetItem(lvItem))
	{
		index = lvItem.lParam;
		HotKey hk = m_pHotkeys->at(index);
		pDialog->GetComboBox()->SetCurSel(hk.command);
	}

	BOOL enable = index == -1 ? FALSE : TRUE;
	pDialog->GetDlgItem(IDC_CLEAR)->EnableWindow(enable);
	pDialog->GetDlgItem(IDC_MODIFY)->EnableWindow(enable);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CommandsCombo::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL HotkeyDialog::OnInitDialog()
{
	TCHAR cmdtext[256] = {0};
	TCHAR restext[256] = {0};

	LoadString(GetApp()->GetInstanceHandle(), COMMANDS[m_hotkey.command].textid, 
		cmdtext, sizeof(cmdtext)/sizeof(cmdtext[0]));
	LoadString(GetApp()->GetInstanceHandle(), IDS_HOTKEY_DIALOG_CAPTION, 
		restext, sizeof(restext)/sizeof(restext[0]));
	
	TCHAR captiontext[256] = {0};
	_stprintf_s(captiontext, 256, restext, cmdtext);
	SetWindowText(captiontext);

	LoadString(GetApp()->GetInstanceHandle(), IDS_HOTKEY_DIALOG_TEXT, 
		restext, sizeof(restext)/sizeof(restext[0]));

	TCHAR labeltext[256] = {0};
	_stprintf_s(labeltext, 256, restext, cmdtext);
	SetDlgItemText(IDC_LABEL, labeltext);

	CenterWindow();
	SetFocus();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL HotkeyDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message != WM_KEYDOWN && pMsg->message != WM_SYSKEYDOWN)
	{
		return CDialog::PreTranslateMessage(pMsg);
	}

	unsigned int key = pMsg->wParam;

	if (key == VK_CONTROL || key == VK_MENU || key == VK_SHIFT || key == VK_RWIN || key == VK_LWIN)
	{
		return TRUE;
	}

	m_hotkey.keycomb.control	= (::GetKeyState(VK_CONTROL) < 0);
	m_hotkey.keycomb.alt		= (::GetKeyState(VK_MENU) < 0);
	m_hotkey.keycomb.shift		= (::GetKeyState(VK_SHIFT) < 0);
	m_hotkey.keycomb.meta		= (::GetKeyState(VK_RWIN) < 0 || ::GetKeyState(VK_LWIN) < 0);

	m_hotkey.keycomb.key		= key;

	OnOK();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitKeyNames()
{
	g_KeyNames.clear();

	char_t str[4] = {0};

	// add 0-9
	for (unsigned int i = 0x30; i < 0x3A; i++)
	{
		str[0] = i;
		g_KeyNames.insert(std::pair<unsigned int, const string_t>(i, string_t(str)));
	}

	// add A-Z
	for (unsigned int i = 0x41; i <= 0x5A; i++)
	{
		str[0] = i;
		g_KeyNames.insert(std::pair<unsigned int, const string_t>(i, string_t(str)));
	}

	str[0] = 'F';
	unsigned int j = 0x31;
	// add F1-F9
	for (unsigned int i = 0x70; i < 0x79; i++)
	{
		str[1] = j++; 
		g_KeyNames.insert(std::pair<unsigned int, const string_t>(i, string_t(str)));
	}

	str[1] = '1';
	j  = 0x30;
	// add F10-F19
	for (unsigned int i = 0x79; i < 0x83; i++)
	{
		str[2] = j++; 
		g_KeyNames.insert(std::pair<unsigned int, const string_t>(i, string_t(str)));
	}

	str[1] = '2';
	j  = 0x30;
	// add F20-F24
	for (unsigned int i = 0x83; i < 0x88; i++)
	{
		str[2] = j++; 
		g_KeyNames.insert(std::pair<unsigned int, const string_t>(i, string_t(str)));
	}

	TCHAR text[256] = {0};

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_SPACE, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_SPACE, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_BACK, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_BACK, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_TAB, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_TAB, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_ESCAPE, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_ESCAPE, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_PRIOR, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_PRIOR, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_NEXT, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_NEXT, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_END, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_END, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_HOME, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_HOME, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_LEFT, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_LEFT, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_UP, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_UP, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_RIGHT, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_RIGHT, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_DOWN, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_DOWN, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_INSERT, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_INSERT, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_DELETE, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_DELETE, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_SNAPSHOT, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_SNAPSHOT, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_PAUSE, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_PAUSE, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_NUMLOCK, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_NUMLOCK, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_SUBTRACT, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_SUBTRACT, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_ADD, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_ADD, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_DIVIDE, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_DIVIDE, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_OEM_COMMA, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_OEM_COMMA, string_t(text)));
	
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_OEM_PERIOD, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_OEM_PERIOD, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_VOLUME_MUTE, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_VOLUME_MUTE, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_VOLUME_DOWN, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_VOLUME_DOWN, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_VOLUME_UP, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_VOLUME_UP, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_MEDIA_NEXT_TRACK, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_MEDIA_NEXT_TRACK, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_MEDIA_PREV_TRACK, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_MEDIA_PREV_TRACK, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_MEDIA_STOP, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_MEDIA_STOP, string_t(text)));

	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_MEDIA_PLAY_PAUSE, text, sizeof(text)/sizeof(text[0]));
	g_KeyNames.insert(std::pair<unsigned int, const string_t>(VK_MEDIA_PLAY_PAUSE, string_t(text)));

	// add num pad 0-9
	LoadString(GetApp()->GetInstanceHandle(), IDS_VK_NUMPADX, text, sizeof(text)/sizeof(text[0]));
	for (unsigned int i = 0x60; i < 0x6A; i++)
	{
		TCHAR keytext[256] = {0};
		_stprintf_s(keytext, 256, text, i - 0x60);
		g_KeyNames.insert(std::pair<unsigned int, const string_t>(i, string_t(keytext)));
	}
}

string_t BuildKeyCombinationString(KeyCombination kc)
{
	string_t str;

	TCHAR text[256] = {0};

	if (kc.control)
	{
		LoadString(GetApp()->GetInstanceHandle(), IDS_VK_CONTROL, text, sizeof(text)/sizeof(text[0]));
		str.append(text).append(TEXT(" + "));
	}

	if (kc.alt)
	{
		LoadString(GetApp()->GetInstanceHandle(), IDS_VK_ALT, text, sizeof(text)/sizeof(text[0]));
		str.append(text).append(TEXT(" + "));
	}

	if (kc.shift)
	{
		LoadString(GetApp()->GetInstanceHandle(), IDS_VK_SHIFT, text, sizeof(text)/sizeof(text[0]));
		str.append(text).append(TEXT(" + "));
	}

	if (kc.meta)
	{
		LoadString(GetApp()->GetInstanceHandle(), IDS_VK_META, text, sizeof(text)/sizeof(text[0]));
		str.append(text).append(TEXT(" + "));
	}

	KeyNamesIterator it = g_KeyNames.find(kc.key);
	if (it != g_KeyNames.end())
	{
		str.append(g_KeyNames[kc.key]);
	}
	else
	{
		TCHAR ftext[256] = {0};
		LoadString(GetApp()->GetInstanceHandle(), IDS_VK_UNKNOWN, ftext, sizeof(ftext)/sizeof(ftext[0]));
		
		ZeroMemory(text, 256);
		_stprintf_s(text, 256, ftext, kc.key);
		str.append(text);
	}

	return str;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
