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

#pragma once

#include <QList>
#include <QMap>

#include "GhKey.h"

class GhPlugin
{
public:
	GhPlugin();

	void registerHotkeys() { registerHotkeys(ghKeys); }
	void unregisterHotkeys() { unregisterHotkeys(registeredActions); }

	void saveHotkeys() { saveHotkeys(ghKeys); }
	void loadHotkeys() { loadHotkeys(ghKeys); }

	const QList<GhKey> hotkeys() const { return ghKeys; }
	void setHotkeys(const QList<GhKey> hotkeys) { ghKeys = hotkeys; }

	void setHWnd(void *wHandle) { hWnd = wHandle; }

	void resetKeys(const QList<GhKey> &hotkeys);

	void handleGlobalKey(const unsigned int wParam) const;

private:
	void *hWnd;
	QList<GhKey> ghKeys;

	QMap<unsigned int, const GhAction*> registeredActions;
	QMap<QString, unsigned int> hotkeysMap;

	unsigned int getFsModifiers(const QStringList &keyTokens);
	unsigned int getVirtualKey(const QStringList &keyTokens);

	void registerHotkeys(const QList<GhKey> &ghKeys);
	void unregisterHotkeys(QMap<unsigned int, const GhAction*> &registeredActions);

	void saveHotkeys(const QList<GhKey> &ghKeys);
	void loadHotkeys(QList<GhKey> &ghKeys);

	void initializeHotkeysMap();

	static QString getConfigFileDir();
	static QString getConfigFilePath();
	static QString getLastWinErrorMessage();
};
