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

#include "GhPlugin.h"
#include "GhAction.h"
#include "GhKey.h"

#include <windows.h>
#include <shlobj.h>
#include <strsafe.h>

static void WinError(LPTSTR lpszFunction);

#include <QAbstractEventDispatcher>
#include <QMessageBox>
#include <QKeySequence>
#include <QString>
#include <QTextStream>
#include <QMapIterator>
#include <QSettings>

GhPlugin::GhPlugin() : registeredActions(), hotkeysMap(), ghKeys()
{
	initializeHotkeysMap();
}

void GhPlugin::registerHotkeys(const QList<GhKey> &ghKeys)
{
	for (int i = 0; i < ghKeys.count(); ++i)
	{
		GhKey key = ghKeys[i];
		QString atomName;
		QTextStream(&atomName) << key.action()->name() << i;
		
#ifdef UNICODE
		ATOM atom = GlobalAddAtom((LPCTSTR)atomName.utf16());
#else
		ATOM atom = GlobalAddAtom((LPCTSTR)atomName);
#endif

		QString keySequenceString = key.keySequence().toString();
		QStringList keyTokens = keySequenceString.split("+");
		
		if (keyTokens.count() == 0)
			continue;

		UINT fsModifiers = getFsModifiers(keyTokens);
		UINT vk = getVirtualKey(keyTokens);

		if (RegisterHotKey((HWND)hWnd, (int) atom, fsModifiers, vk))
			registeredActions[(int) atom] = key.action();
		else {
			QString errorMessage;
			QTextStream(&errorMessage) << "Could not register hotkey "
				<< keySequenceString << " for action " << key.action()->name() << ".\n"
				<< "Error " << GetLastError() << ": " << getLastWinErrorMessage();
			
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.setWindowTitle("Global Hotkeys Plugin");
			msgBox.setText(errorMessage);
			msgBox.exec();
		}
	}
}

void GhPlugin::unregisterHotkeys(QMap<unsigned int, const GhAction*> &registeredActions)
{
	QMap<unsigned int, const GhAction*>::iterator i;
	for (i = registeredActions.begin(); i != registeredActions.end(); ++i) {
		UnregisterHotKey((HWND)hWnd, i.key());
		GlobalDeleteAtom(ATOM(i.key()));	
	}
	
	registeredActions.clear();
}

void GhPlugin::resetKeys(const QList<GhKey> &hotkeys)
{
	unregisterHotkeys();
	ghKeys = hotkeys;
	registerHotkeys();
	saveHotkeys();
}

UINT GhPlugin::getFsModifiers(const QStringList &keyTokens)
{
	UINT modifiers = 0;

	for (int i = 0; i < keyTokens.count() - 1; i++)
	{
		QString token = keyTokens[i];
		
		if (token == "Ctrl") {
			modifiers |= MOD_CONTROL;
			continue;
		}

		if (token == "Alt") {
			modifiers |= MOD_ALT;
			continue;
		}

		if (token == "Shift") {
			modifiers |= MOD_SHIFT;
			continue;
		}

		if (token == "Meta") {
			modifiers |= MOD_WIN;
			continue;
		}
	}

	return modifiers;
}

UINT GhPlugin::getVirtualKey(const QStringList &keyTokens)
{
	UINT vk = 0;
	QString token = keyTokens[keyTokens.count() - 1];

	if (hotkeysMap.contains(token))
		vk = hotkeysMap[token];

	return vk;
}

void GhPlugin::handleGlobalKey(const unsigned int wParam) const
{
	if (registeredActions.contains(wParam))
		registeredActions[wParam]->execute();
}

void GhPlugin::saveHotkeys(const QList<GhKey> &ghKeys)
{
	QSettings settings(getConfigFilePath(), QSettings::IniFormat);
	
	settings.clear();
	settings.sync();

	settings.beginWriteArray("hotkeys");
	for (int i = 0; i < ghKeys.count(); ++i) {
		settings.setArrayIndex(i);
		settings.setValue("action", ghKeys[i].action()->name());
		settings.setValue("hotkey", ghKeys[i].keySequence().toString());
	}
	settings.endArray();
	settings.sync();
}

void GhPlugin::loadHotkeys(QList<GhKey> &ghKeys)
{
	QSettings settings(getConfigFilePath(), QSettings::IniFormat);
	
	int size = settings.beginReadArray("hotkeys");
	for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);

		int actionIndex = -1;
		QString actionName = settings.value("action").toString();
		for (int j = 0; j < GhAction::Actions.count(); ++j) {
			const GhAction *cAction = GhAction::Actions.at(j);
			if (cAction->name() == actionName) {
				actionIndex = j;
				break;
			}
		}

		if (actionIndex == -1) {
			QString errorMessage;
			QTextStream(&errorMessage) << "Unknown action: " << actionName 
				<< ". Action will not be loaded.";
			
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.setWindowTitle("Global Hotkeys Plugin");
			msgBox.setText(errorMessage);
			msgBox.exec();

			continue;
		}

		QKeySequence keySequence = QKeySequence(settings.value("hotkey").toString());
		ghKeys.append(GhKey(GhAction::Actions[actionIndex], keySequence));

	}
	settings.endArray();

	bool openDialogActionDefined = false;
	for (int i = 0; i < ghKeys.count(); ++i) {
		GhKey key = ghKeys[i];
		QString actionName = key.action()->name();
		if (actionName == GhAction::Actions[0]->name())
			openDialogActionDefined = true;
	}

	if (!openDialogActionDefined) {
		ghKeys.prepend(GhKey(GhAction::Actions[0], QKeySequence("Ctrl+Shift+P")));
	}
}

void GhPlugin::initializeHotkeysMap()
{
	char str[4];
	for (unsigned int i = 0; i < 4; i++)
		str[i] = '\0';

	// add 0-9
	for (unsigned int i = 0x30; i < 0x3A; i++) {
		str[0] = i; 
		hotkeysMap[QString(str)] = i;
	}

	// add A-Z
	for (unsigned int i = 0x41; i <= 0x5A; i++) {
		str[0] = i; 
		hotkeysMap[QString(str)] = i;
	}

	// add a-z
	unsigned int j  = 0x61;
	for (unsigned int i = 0x41; i <= 0x5A; i++) {
		str[0] = j++; 
		hotkeysMap[QString(str)] = i;
	}

	str[0] = 'F';
	j  = 0x31;
	// add F1-F9
	for (unsigned int i = 0x70; i < 0x79; i++) {
		str[1] = j++; 
		hotkeysMap[QString(str)] = i;
	}

	str[1] = '1';
	j  = 0x30;
	// add F10-F19
	for (unsigned int i = 0x79; i < 0x83; i++) {
		str[2] = j++; 
		hotkeysMap[QString(str)] = i;
	}

	str[1] = '2';
	j  = 0x30;
	// add F20-F24
	for (unsigned int i = 0x83; i < 0x88; i++) {
		str[2] = j++; 
		hotkeysMap[QString(str)] = i;
	}

	hotkeysMap["Space"] = VK_SPACE;
	hotkeysMap["Backspace"] = VK_BACK;
	hotkeysMap["Tab"] = VK_TAB;
	hotkeysMap["Escape"] = VK_ESCAPE;
	
	hotkeysMap["PageUp"] = VK_PRIOR;
	hotkeysMap["PageDown"] = VK_NEXT;
	
	hotkeysMap["End"] = VK_END;
	hotkeysMap["Home"] = VK_HOME;

	hotkeysMap["Left"] = VK_LEFT;
	hotkeysMap["Up"] = VK_UP;
	hotkeysMap["Right"] = VK_RIGHT;
	hotkeysMap["Down"] = VK_DOWN;

	hotkeysMap["Insert"] = VK_INSERT;
	hotkeysMap["Delete"] = VK_DELETE;

	hotkeysMap["PrintScreen"] = VK_SNAPSHOT;
	hotkeysMap["Pause"] = VK_PAUSE;
	hotkeysMap["NumLock"] = VK_NUMLOCK;

	hotkeysMap["-"] = VK_SUBTRACT;
	hotkeysMap["="] = VK_ADD;
	hotkeysMap["/"] = VK_DIVIDE;
	hotkeysMap[","] = VK_OEM_COMMA;
	hotkeysMap["."] = VK_OEM_PERIOD;

	hotkeysMap["MM_PlayPause"] = VK_MEDIA_PLAY_PAUSE;
	hotkeysMap["MM_Previous"] = VK_MEDIA_PREV_TRACK;
	hotkeysMap["MM_Next"] = VK_MEDIA_NEXT_TRACK;
	hotkeysMap["MM_VolumeDown"] = VK_VOLUME_DOWN;
	hotkeysMap["MM_VolumeUp"] = VK_VOLUME_UP;
	hotkeysMap["MM_Stop"] = VK_MEDIA_STOP;
}

QString GhPlugin::getLastWinErrorMessage()
{
	QString message;
	LPVOID lpMsgBuf;
    DWORD dw = GetLastError();

	FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

#ifdef UNICODE
	message.append(QString::fromWCharArray((LPTSTR) lpMsgBuf));
#else
	message.append((LPTSTR) lpMsgBuf);
#endif

	LocalFree(lpMsgBuf);

	return message;
}

QString GhPlugin::getConfigFileDir()
{
	QString configFileDir;
	OSVERSIONINFO osvi;
	
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	TCHAR path[MAX_PATH];
	ZeroMemory(path, sizeof(TCHAR) * MAX_PATH);

	if (osvi.dwMajorVersion > 5) {
		// Vista and above
		SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);

#ifdef UNICODE
		configFileDir.append(QString::fromWCharArray(path));
#else
		configFileDir.append(path);
#endif

		configFileDir.append("\\Apple Computer");
	} else {
		// XP and 2000
		SHGetFolderPath(NULL, CSIDL_MYMUSIC, NULL, 0, path);

#ifdef UNICODE
		configFileDir.append(QString::fromWCharArray(path));
#else
		configFileDir.append(path);
#endif

	}

	configFileDir.append("\\iTunes\\iTunes Plug-ins");

	return configFileDir;
}

QString GhPlugin::getConfigFilePath()
{
	QString configFileDir = getConfigFileDir();
	
	QString configFilePath;
	if (!configFileDir.isEmpty())
		configFilePath.append(configFileDir).append("\\");
	configFilePath.append("GlobalHotkeysConfig.ini");

	return configFilePath;
}
