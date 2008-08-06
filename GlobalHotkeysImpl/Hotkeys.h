#ifndef HOTKEYS_H
#define HOTKEYS_H

#include <windows.h>
#include <string>

class Hotkey
{
public:
	Hotkey();
	~Hotkey();

	void PerformAction();	

private:
	unsigned int key;
	bool alt;
	bool control;
	bool shift;
	bool win;

	void (*Action)();

};

void RegisterGlobalKeys(HWND hwnd);
void UnregisterGlobalKeys(HWND hwnd);

void HandleGlobalKey(WPARAM wParam, LPARAM lParam);


#endif /* HOTKEYS_H */