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

#include "Actions.h"

std::map<const std::string, Actions> actionsMap = std::map<const std::string, Actions>();

void InitActionsMap()
{
	actionsMap["PlayPause"] = eActionPlayPause;
	actionsMap["NextTrack"] = eActionNextTrack;
	actionsMap["PreviousTrack"] = eActionPreviousTrack;
	actionsMap["ToggleRandom"] = eActionRandom;
	actionsMap["ToggleRepeat"] = eActionRepeat;
	actionsMap["SongRatingClear"] = eActionSongRatingClear;
	actionsMap["SongRating1"] = eActionSongRating1;
	actionsMap["SongRating2"] = eActionSongRating2;
	actionsMap["SongRating3"] = eActionSongRating3;
	actionsMap["SongRating4"] = eActionSongRating4;
	actionsMap["SongRating5"] = eActionSongRating5;
	actionsMap["ShowHide"] = eActionShowHide;
}

void PlayPause()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->PlayPause();
		iITunes->Release();
	}

	CoUninitialize();
}

void NextTrack()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->NextTrack();
		iITunes->Release();
	}

	CoUninitialize();

}

void PreviousTrack()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->PreviousTrack();
		iITunes->Release();
	}

	CoUninitialize();
}

void Random()
{
	IiTunes* iITunes = 0;
	IITPlaylist* iIPlaylist = 0;
	HRESULT hRes;
	VARIANT_BOOL suffle;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->get_CurrentPlaylist(&iIPlaylist);
		if (iIPlaylist) {
			iIPlaylist->get_Shuffle(&suffle);
			
			if (suffle)
				iIPlaylist->put_Shuffle(0); // 0 == false
			else
				iIPlaylist->put_Shuffle(-1);// -1 == true

			iIPlaylist->Release();
		}
		
		iITunes->Release();
	}

	CoUninitialize();

}

void Repeat()
{
	IiTunes* iITunes = 0;
	IITPlaylist* iIPlaylist = 0;
	HRESULT hRes;
	ITPlaylistRepeatMode repeatMode;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->get_CurrentPlaylist(&iIPlaylist);
		if (iIPlaylist) {
			iIPlaylist->get_SongRepeat(&repeatMode);
			
			switch(repeatMode) {
				case ITPlaylistRepeatModeOff:
					repeatMode = ITPlaylistRepeatModeAll;
					break;
				case ITPlaylistRepeatModeAll:
					repeatMode = ITPlaylistRepeatModeOne;
					break;
				case ITPlaylistRepeatModeOne:
					repeatMode = ITPlaylistRepeatModeOff;
					break;
			}
			
			iIPlaylist->put_SongRepeat(repeatMode);
			

			iIPlaylist->Release();
		}

		iITunes->Release();
	}

	CoUninitialize();

}

void RateSong(unsigned int rating)
{
	IiTunes* iITunes = 0;
	IITTrack* iITTrack = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->get_CurrentTrack(&iITTrack);

		if(iITTrack) {
			iITTrack->put_Rating(rating);
			iITTrack->Release();
		}

		iITunes->Release();
	}

	CoUninitialize();
}

void ClearSongRating()
{
	RateSong(0);
}

void SongRating1()
{
	RateSong(20);
}

void SongRating2()
{
	RateSong(40);
}

void SongRating3()
{
	RateSong(60);
}

void SongRating4()
{
	RateSong(80);
}

void SongRating5()
{
	RateSong(100);
}

void ShowHide()
{
	IiTunes* iITunes = 0;
	IITBrowserWindow* iITBrowserWindow = 0; 
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->get_BrowserWindow(&iITBrowserWindow);

		if(iITBrowserWindow) {
			VARIANT_BOOL isVisible = 0;
			iITBrowserWindow->get_Minimized(&isVisible);

			isVisible = (isVisible == 0) ? -1 : 0;
			iITBrowserWindow->put_Minimized(isVisible);

			iITBrowserWindow->Release();
		}

		iITunes->Release();
	}

	CoUninitialize();
}