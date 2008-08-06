#include <windows.h>
#include <tinyxml.h>

#include "iTunesCOMInterface.h"
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
	TiXmlDocument configFile("GlobalHotkeysConfig.xml");
	bool loadOk = configFile.LoadFile();
	
	if (loadOk) {
		const TiXmlElement* root = configFile.RootElement();

		const TiXmlElement* element = root->FirstChildElement();
		while (element) {
			// TODO: Found new global key
			const TiXmlAttribute* attrib = element->FirstAttribute();
			while(attrib) {
				// TODO: Found new attribute
				attrib = attrib->Next();
			}
			element = element->NextSiblingElement();
		}
	} 

	return loadOk;
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