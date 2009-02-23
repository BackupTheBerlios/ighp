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

#include "PluginSettings.h" 

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <shlobj.h>

#include <tinyxml\tinyxml.h>

PluginSettings* PluginSettings::ms_instance = 0;

PluginSettings* PluginSettings::Instance()
{
	if (ms_instance == 0)
		ms_instance = new PluginSettings();
	return ms_instance;
}

void PluginSettings::Destroy()
{
	if (ms_instance != 0) {
		delete ms_instance;
		ms_instance = 0;
	}
}

PluginSettings::PluginSettings() : 
	m_configFile(std::string("GlobalHotkeysConfig.xml")), 
	m_keyId(-1)
{
	m_hotkeys = new std::map<const unsigned int, Hotkey*>();

	::InitActionsMap();
	::InitHotkeysMap();

	AddDefaultHotkeys();
}

PluginSettings::~PluginSettings()
{
	delete m_hotkeys;
}

std::map<const unsigned int, Hotkey*>* PluginSettings::GetHotkeys()
{
	return m_hotkeys;
}

bool PluginSettings::ReadConfigFile(std::map<const unsigned int, Hotkey*>* hotkeys)
{
	std::string configFilePath = std::string("");
	if (!GetConfigFile(&configFilePath))
		return false;

	TiXmlDocument configFile(configFilePath.c_str());
	if (!configFile.LoadFile())
		return false;

	m_keyId = -1;
	hotkeys->clear();

	const TiXmlElement* root = configFile.RootElement();

	const TiXmlElement* element = root->FirstChildElement();
	while (element) {
		std::string action_name = std::string(element->Attribute("action") != 0 ? 
			element->Attribute("action") : "NotDefined");
		std::string key_name = std::string(element->Attribute("key") != 0 ? 
			element->Attribute("key") : "NotDefined");
		std::string alt_str = std::string(element->Attribute("alt") != 0 ? 
			element->Attribute("alt") : "NotDefined");
		std::string control_str = std::string(element->Attribute("control") != 0 ? 
			element->Attribute("control") : "NotDefined");
		std::string shift_str = std::string(element->Attribute("shift") != 0 ? 
			element->Attribute("shift") : "NotDefined");
		std::string win_str = std::string(element->Attribute("win") != 0 ? 
			element->Attribute("win") : "NotDefined");

		(*hotkeys)[++m_keyId] = new Hotkey(action_name, key_name, alt_str, control_str, shift_str, win_str);

		element = element->NextSiblingElement();
	}

	return true;
}

void PluginSettings::AddDefaultHotkeys()
{
	std::map<const unsigned int, Hotkey*>& hotkeys = *this->GetHotkeys();

	// Settings Dialog 
	// hotkeys[++m_keyId] = new Hotkey("OpenSettingsDialog", "P", "false", "true", "true", "false");
}

bool PluginSettings::WriteConfigFile(std::map<const unsigned int, Hotkey*>* hotkeys)
{
	std::string configFilePath = std::string("");
	if (!GetConfigFile(&configFilePath))
		return false;

	// TODO: Implement this

	return false;	
}

bool PluginSettings::GetConfigFile(std::string* str)
{
	OSVERSIONINFO osvi;
	
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	char path[MAX_PATH];
	ZeroMemory(path, sizeof(char) * MAX_PATH);

	if (osvi.dwMajorVersion > 5) {
		// Vista and above
		SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
		str->append(path);
		str->append("\\Apple Computer");
	} else {
		// XP and 2000
		SHGetFolderPath(NULL, CSIDL_MYMUSIC, NULL, 0, path);
		str->append(path);
	}

	str->append("\\iTunes\\iTunes Plug-ins\\");
	str->append(m_configFile);

	return true;
}