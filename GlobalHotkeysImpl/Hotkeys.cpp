#include "Hotkeys.h"
#include "Actions.h"

Hotkey::Hotkey()
{

}

Hotkey::~Hotkey()
{

}

void Hotkey::PerformAction()
{
	if(Action > 0)
		Action();
}

void RegisterGlobalKeys(HWND hwnd)
{
	RegisterHotKey(hwnd, 1, MOD_ALT, VK_SPACE);
}

void UnregisterGlobalKeys(HWND hwnd)
{
	UnregisterHotKey(hwnd, 1);
}

void HandleGlobalKey(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 1:
		PlayPause();
		break;
	case 2:
		NextTrack();
		break;
	case 3:
		PreviousTrack();
		break;
	}
}