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

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ConfigDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
    {
		case IDAPPLY:
		{
			OnApply();
			return TRUE;
		}
    }

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConfigDialog::OnOK()
{
	OnApply();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConfigDialog::OnApply()
{
	SaveHotkeys();
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

	for (HotKeysIterator it = m_pHotkeys->begin(); it != m_pHotkeys->end(); ++it)
	{
		HotKey hk = (*it);

		TCHAR text[256] = {0};

		::LoadString(GetApp()->GetInstanceHandle(), COMMANDS[hk.command].textid,
				text, sizeof(text)/sizeof(text[0]));

		LVITEM lvItem;
		ZeroMemory(&lvItem, sizeof(LVITEM));

		lvItem.mask		= LVIF_TEXT;
		lvItem.pszText	= (LPTSTR) text;

		int item = InsertItem(lvItem);
		
		const string_t str = BuildKeyCombinationString(hk.keycomb);
		ZeroMemory(&lvItem, sizeof(LVITEM));

		lvItem.mask		= LVIF_TEXT;
		lvItem.iItem	= item;
		lvItem.iSubItem	= 1;
		lvItem.pszText	= (LPTSTR) str.c_str();

		SendMessage(LVM_SETITEM, 0, (LPARAM)&lvItem);
	}

	int count = GetItemCount();
	count++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HotkeysListView::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | LVS_AUTOARRANGE |
            LVS_ICON | LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS;
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT HotkeysListView::OnNotifyReflect(WPARAM wParam, LPARAM lParam)
{
	LPNMHDR  pnmh = (LPNMHDR) lParam;

	switch(pnmh->code)
	{
		case NM_RCLICK:
		{
			//get the item that has the focus
			int item = (int)::SendMessage(m_hWnd, LVM_GETNEXTITEM, (WPARAM) -1, (LPARAM) MAKELPARAM (LVNI_FOCUSED, 0));
			break;
		}

		case NM_DBLCLK:
		case NM_RETURN:
		{
			//get the item that has the focus
			int item = (int)::SendMessage(m_hWnd, LVM_GETNEXTITEM, (WPARAM) -1, (LPARAM) MAKELPARAM (LVNI_FOCUSED, 0));
			break;
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CommandsCombo::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN;
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
		ZeroMemory(text, 256);
		_stprintf_s(text, 256, TEXT("%#x"), kc.key);
		str.append(text);
	}

	return str;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
