#include "Actions.h"

std::map<const std::string, Actions> actionsMap = std::map<const std::string, Actions>();

void InitActionsMap()
{
	actionsMap["PlayPause"] = eActionPlayPause;
	actionsMap["NextTrack"] = eActionNextTrack;
	actionsMap["PreviousTrack"] = eActionPreviousTrack;
}

void PlayPause()
{
	IiTunes *iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *)&iITunes);

	if(hRes == S_OK && iITunes)
	{

		iITunes->PlayPause();
		iITunes->Release();
	}

	CoUninitialize();
}

void NextTrack()
{
	IiTunes *iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *)&iITunes);

	if(hRes == S_OK && iITunes)
	{

		iITunes->NextTrack();
		iITunes->Release();
	}

	CoUninitialize();

}

void PreviousTrack()
{
	IiTunes *iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *)&iITunes);

	if(hRes == S_OK && iITunes)
	{

		iITunes->PreviousTrack();
		iITunes->Release();
	}

	CoUninitialize();
}