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
#include "HotKeyManager.h"

#include <shlobj.h>
#include <json\json.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool HotKeyManager::IsDefined(HotKey otherhk)
{
	for (HotKeysIterator it = m_hotkeys.begin(); it != m_hotkeys.end(); ++it)
	{
		HotKey hk = (*it);

		if (hk.keycomb == otherhk.keycomb)
		{
			return true;
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HotKeyManager::SaveHotkeys()
{
	using namespace Json;

	string_t configFile = GetConfigFilePath();
	HANDLE hFile = CreateFile(configFile.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		// TODO: Report error
		OutputDebugString(TEXT("Could not open file for writing\n"));
		return;
	}

	Value root;
	
	Value hotkeysobj;
	for (HotKeysIterator it = m_hotkeys.begin(); it != m_hotkeys.end(); ++it)
	{
		HotKey hk = (*it);
		
		Value hkobj;
		Value keycombobj;

		keycombobj["key"]		= hk.keycomb.key;
		keycombobj["alt"]		= hk.keycomb.alt;
		keycombobj["control"]	= hk.keycomb.control;
		keycombobj["shift"]		= hk.keycomb.shift;
		keycombobj["meta"]		= hk.keycomb.meta;

		hkobj["command"]		= hk.command;
		hkobj["keycomb"]		= keycombobj;

		hotkeysobj.append(hkobj);
	}

	root["hotkeys"] = hotkeysobj;

	StyledWriter writer;
	std::string configDoc = writer.write(root);

	unsigned long writesize = 0;
	WriteFile(hFile, configDoc.c_str(), configDoc.size(), &writesize, NULL);
	CloseHandle(hFile);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HotKeyManager::LoadHotkeys()
{
	using namespace Json;

	string_t configFile = GetConfigFilePath();
	HANDLE hFile = CreateFile(configFile.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		// TODO: Report error
		OutputDebugString(TEXT("Could not open file for reading\n"));
		CheckForSettingsSialog();
		return;
	}

	unsigned long bufsize	= GetFileSize(hFile, 0);
	unsigned long bytesRead = 0;
	
	if (bufsize == INVALID_FILE_SIZE)
	{
		// TODO: Report error
		OutputDebugString(TEXT("Could not get file size\n"));
		CheckForSettingsSialog();
		return;
	}

	char* ReadBuffer = new char[bufsize];
	ZeroMemory(ReadBuffer, bufsize * sizeof(char));

	if (ReadFile(hFile, ReadBuffer, bufsize - 1, &bytesRead, NULL) == FALSE)
	{
		// TODO: Report error
		OutputDebugString(TEXT("Could not read file\n"));
		CheckForSettingsSialog();
		return;
	}

	if (bytesRead <= 0 || bytesRead > bufsize)
	{
		// TODO: Report error
		OutputDebugString(TEXT("No data read\n"));
		CheckForSettingsSialog();
		return;
	}

	CloseHandle(hFile);

	Value root;
	Reader reader;

	if (!reader.parse(std::string(ReadBuffer), root))
	{
		// TODO: Report error
		OutputDebugString(TEXT("Error while reading config file\n"));
		CheckForSettingsSialog();
		return;
	}

	const Value hotkeys = root["hotkeys"];
	for (size_t i = 0; i < hotkeys.size(); ++i)
	{
		Value hotkey = hotkeys[i];
		Value keycomb = hotkey["keycomb"];

		HotKey hk;	
		ZeroMemory(&hk, sizeof(HotKey));

		hk.command			= static_cast<eCommand>(hotkey["command"].asUInt());
		hk.keycomb.key		= keycomb["key"].asUInt();
		hk.keycomb.alt		= keycomb["alt"].asBool();
		hk.keycomb.control	= keycomb["control"].asBool();
		hk.keycomb.shift	= keycomb["shift"].asBool();
		hk.keycomb.meta		= keycomb["meta"].asBool();

		m_hotkeys.push_back(hk);
	}

	SAFE_DEL_ARRAY(ReadBuffer);
	CheckForSettingsSialog();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HotKeyManager::CheckForSettingsSialog()
{
	// Check for settings dialog hotkey
	bool hasConfigDialogShortcut = false;
	for (HotKeysIterator it = m_hotkeys.begin(); it != m_hotkeys.end(); ++it)
	{
		HotKey hk = (*it);

		if (hk.command == CMD_OPEN_CONFIG_DIALOG)
		{
			hasConfigDialogShortcut = true;
			break;
		}
	}

	if (!hasConfigDialogShortcut)
	{
		// add default settings dialog hotkey: Ctrl + Shift + P
		HotKey hk;
		ZeroMemory(&hk, sizeof(HotKey));

		hk.command			= CMD_OPEN_CONFIG_DIALOG;
		hk.keycomb.control	= true;
		hk.keycomb.shift	= true;
		hk.keycomb.key		= 0x50; // P

		m_hotkeys.push_back(hk);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const string_t HotKeyManager::GetConfigFileDir() const
{
	string_t dir;

	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	TCHAR path[MAX_PATH];
	ZeroMemory(path, sizeof(TCHAR) * MAX_PATH);

	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
	dir.append(path);
	dir.append(TEXT("\\Apple Computer\\iTunes\\iTunes Plug-ins"));

	return dir;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const string_t HotKeyManager::GetConfigFilePath() const
{
	string_t path;

	path.append(GetConfigFileDir());

	if (path.length() != 0)
	{
		path.append(TEXT("\\"));
	}

	path.append(TEXT("GlobalHotkeys.config"));
	return path;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
