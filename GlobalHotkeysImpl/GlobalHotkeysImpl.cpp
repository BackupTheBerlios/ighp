#include <windows.h>
#include <tinyxml.h>

#include "iTunesCOMInterface.h"

#include "Actions.h"
#include "Hotkeys.h"

LRESULT CALLBACK WindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return true;
}

bool ReadConfigFile()
{
	// TODO: Determine the path
	TiXmlDocument configFile("c:\\Program Files\\iTunes\\Plug-Ins\\GlobalHotkeysConfig.xml");
	bool loadOk = configFile.LoadFile();
	
	if (loadOk) {
		const TiXmlElement* root = configFile.RootElement();
		unsigned int keyId = 0;

		const TiXmlElement* element = root->FirstChildElement();
		while (element) {
			std::string action_name = std::string(element->Attribute("action") != 0 ? element->Attribute("action") : "NotDefined");
			std::string key_name = std::string(element->Attribute("key") != 0 ? element->Attribute("key") : "NotDefined");
			std::string alt_str = std::string(element->Attribute("alt") != 0 ? element->Attribute("alt") : "NotDefined");
			std::string control_str = std::string(element->Attribute("control") != 0 ? element->Attribute("control") : "NotDefined");
			std::string shift_str = std::string(element->Attribute("shift") != 0 ? element->Attribute("shift") : "NotDefined");
			std::string win_str = std::string(element->Attribute("win") != 0 ? element->Attribute("win") : "NotDefined");

			hotkey[++keyId] = new Hotkey(action_name, key_name, alt_str, control_str, shift_str, win_str);

			element = element->NextSiblingElement();
		}
	}

	return loadOk;
}

void RegisterGlobalKeys(HWND hwnd)
{
	std::map<const unsigned int, Hotkey*>::iterator iter;
	for (iter = hotkey.begin(); iter != hotkey.end(); iter++) {
		unsigned int modifiers = (iter->second->GetAlt() ? MOD_ALT : 0) | 
			                     (iter->second->GetControl() ? MOD_CONTROL : 0) | 
			                     (iter->second->GetShift() ? MOD_SHIFT : 0) | 
			                     (iter->second->GetWin() ? MOD_WIN : 0);

		RegisterHotKey(hwnd, iter->first, modifiers, iter->second->GetKeyCode());
		
	}
}

void UnregisterGlobalKeys(HWND hwnd)
{
	std::map<const unsigned int, Hotkey*>::iterator iter;

	for (iter = hotkey.begin(); iter != hotkey.end(); iter++) {
		UnregisterHotKey(hwnd, iter->first);
		delete iter->second;
		iter->second = 0;
	}
}

void HandleGlobalKey(WPARAM wParam, LPARAM lParam)
{
	hotkey[wParam]->PerformAction();
}

extern "C" void WINAPI Initialize()
{
	// Write here all the code you need to initialize the DLL
}

extern "C" void WINAPI Release()
{
	// Write here all the code you need to free everything ...
}

extern "C" void WINAPI InitGlobalHotkeysPlugin()
{
	HWND hwnd;
	WNDCLASS wc;
	TCHAR szAppName [] = TEXT("GlobalHotkeysClass");

	wc.style = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppName;

	RegisterClass (&wc);

	hwnd = CreateWindowEx (WS_EX_TRANSPARENT, szAppName, TEXT("Global Hotkeys"),  WS_POPUP, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL) ; 
}

LRESULT CALLBACK WindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			InitActionsMap();
			InitHotkeysMap();
			ReadConfigFile();
			RegisterGlobalKeys(hwnd);
			break;
		case WM_HOTKEY:
			HandleGlobalKey(wParam, lParam);
			break;
		case WM_DESTROY:
			UnregisterGlobalKeys(hwnd);
			PostQuitMessage (0);
			return 0;
	}

	return DefWindowProc (hwnd, message, wParam, lParam);
}