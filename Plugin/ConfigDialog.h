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

#include "HotKeyManager.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <dialog.h>
#include <controls.h>
#include <listview.h>

#include "resource.h"

class HotkeysListView : public CListView
{
public:
	HotkeysListView()			{ };
	virtual ~HotkeysListView()	{ };

	void SetHotkeysList(HotKeys* pHotkeys) { m_pHotkeys = pHotkeys; }
	void PopulateList();

	int GetSelectedItem() { return (int)::SendMessage(m_hWnd, LVM_GETNEXTITEM, (WPARAM) -1, (LPARAM) MAKELPARAM(LVNI_FOCUSED, 0)); }

	void InsertHotkey(HotKey hk);
	void UpdateHotkey(int item, KeyCombination keycomb);

private:
	HotKeys* m_pHotkeys;

	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);

	void UpdateGuiFromSelection();
};

class CommandsCombo : public CComboBox
{
public:
	CommandsCombo()				{ };
	virtual ~CommandsCombo()	{ };
	virtual void PreCreate(CREATESTRUCT &cs);
};

class ConfigDialog : public CDialog, public HotKeyManager
{
public:
	ConfigDialog(CWnd* pParent = NULL) : CDialog(IDD_CONFIG_DIALOG, pParent) { };
	virtual ~ConfigDialog() { };

	CListView* GetListView() {return &m_listView; }
	CComboBox* GetComboBox() {return &m_comboBox; }

private:
	HotkeysListView	m_listView;
	CommandsCombo	m_comboBox;
	
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

	void OnApply();
	void OnAdd();
	void OnRemove();
	void OnModify();
};

class HotkeyDialog : public CDialog
{
public:
	HotkeyDialog(eCommand command, CWnd* pParent = NULL) : CDialog(IDD_HOTKEY_DIALOG, pParent) { m_hotkey.command = command; };
	virtual ~HotkeyDialog() { };

	HotKey GetHotKey() { return m_hotkey; }

private:
	HotKey m_hotkey; 

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
