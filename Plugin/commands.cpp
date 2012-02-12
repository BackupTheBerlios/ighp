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

#include "stdafx.h"
#include "commands.h"

#include <comutil.h>
#include <iTunesCOMInterface.h>

void ShowErrorMessage()
{

}

void PlayPause()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes)
	{
		iITunes->PlayPause();
	}
	else
	{
		ShowErrorMessage();
	}

	SAFE_RELEASE(iITunes);
	CoUninitialize();
}


void Stop()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes)
	{
		iITunes->Stop();
		iITunes->Release();
	}
	else
	{
		ShowErrorMessage();
	}

	SAFE_RELEASE(iITunes);
	CoUninitialize();
}

void NextTrack()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) 
	{
		iITunes->NextTrack();
		iITunes->Release();
	}
	else
	{
		ShowErrorMessage();
	}

	SAFE_RELEASE(iITunes);
	CoUninitialize();
}

void PreviousTrack()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes)
	{
		iITunes->PreviousTrack();
	}
	else
	{
		ShowErrorMessage();
	}

	SAFE_RELEASE(iITunes);
	CoUninitialize();
}

void ToggleRandom()
{
	IiTunes* iITunes = 0;
	IITPlaylist* iIPlaylist = 0;
	HRESULT hRes;
	VARIANT_BOOL suffle;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes)
	{
		iITunes->get_CurrentPlaylist(&iIPlaylist);
		if (iIPlaylist) {
			iIPlaylist->get_Shuffle(&suffle);
			
			if (suffle)
			{
				iIPlaylist->put_Shuffle(0); // 0 == false
			}
			else
			{
				iIPlaylist->put_Shuffle(-1);// -1 == true
			}
		}
	}
	else
	{
		ShowErrorMessage();
	}

	SAFE_RELEASE(iIPlaylist);
	SAFE_RELEASE(iITunes);
	CoUninitialize();
}

void ToggleRepeat()
{
	IiTunes* iITunes = 0;
	IITPlaylist* iIPlaylist = 0;
	HRESULT hRes;
	ITPlaylistRepeatMode repeatMode;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes)
	{
		iITunes->get_CurrentPlaylist(&iIPlaylist);
		if (iIPlaylist)
		{
			iIPlaylist->get_SongRepeat(&repeatMode);
			
			switch(repeatMode)
			{
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
		}
	}
	else
	{
		ShowErrorMessage();
	}

	SAFE_RELEASE(iIPlaylist);
	SAFE_RELEASE(iITunes);
	CoUninitialize();
}

void ShowHide()
{
	IiTunes* iITunes = 0;
	IITBrowserWindow* iITBrowserWindow = 0; 
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes)
	{
		iITunes->get_BrowserWindow(&iITBrowserWindow);

		if(iITBrowserWindow)
		{
			VARIANT_BOOL isVisible = 0;
			iITBrowserWindow->get_Minimized(&isVisible);

			isVisible = (isVisible == 0) ? -1 : 0;
			iITBrowserWindow->put_Minimized(isVisible);
		}
	}
	else
	{
		ShowErrorMessage();
	}

	SAFE_RELEASE(iITBrowserWindow);
	SAFE_RELEASE(iITunes);
	CoUninitialize();
}

void ToggleVolume(const long step)
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes)
	{
		long volume = 50;
		iITunes->get_SoundVolume(&volume);

		volume += step;
		iITunes->put_SoundVolume(volume);
	}
	else
	{
		ShowErrorMessage();
	}

	SAFE_RELEASE(iITunes);
	CoUninitialize();
}

void VolumeUp()
{
	ToggleVolume(5);
}

void VolumeDown()
{
	ToggleVolume(-5);
}

void ToggleMute()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes)
	{
		VARIANT_BOOL isMuted = 0;
		iITunes->get_Mute(&isMuted);
		
		isMuted = (isMuted == 0) ? -1 : 0;
		iITunes->put_Mute(isMuted);
	}
	else
	{
		ShowErrorMessage();
	}

	SAFE_RELEASE(iITunes);
	CoUninitialize();
}

void RateSong(const unsigned int rating)
{
	IiTunes* iITunes = 0;
	IITTrack* iITTrack = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes)
	{
		iITunes->get_CurrentTrack(&iITTrack);

		if(iITTrack)
		{
			iITTrack->put_Rating(rating);
		}
	} 
	else
	{
		ShowErrorMessage();
	}

	SAFE_RELEASE(iITTrack);
	SAFE_RELEASE(iITunes);
	CoUninitialize();
}

void SongRatingClear()
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

const Command COMMANDS [ CMD_COUNT ] = 
{
	{ /* CMD_PLAYPAUSE */				TEXT("Play/Pause"),				&PlayPause			},
	{ /* CMD_STOP */					TEXT("Stop"),					&Stop				},
	{ /* CMD_NEXT_TRACK */				TEXT("Next Track"),				&NextTrack			},
	{ /* CMD_PREVIOUS_TRACK */			TEXT("Previous Track"),			&PreviousTrack		},
	{ /* CMD_TOGGLE_RANDOM */			TEXT("Toggle Random"),			&ToggleRandom		},
	{ /* CMD_TOGGLE_REPEAT */			TEXT("Toggle Repeat"),			&ToggleRepeat		},
	{ /* CMD_SHOWHIDE */				TEXT("Show/Hide"),				&ShowHide			},
	{ /* CMD_VOLUME_UP */				TEXT("Increase Volume"),		&VolumeUp			},
	{ /* CMD_VOLUME_DOWN */				TEXT("Decrease Volume"),		&VolumeDown			},
	{ /* CMD_TOGGLE_MUTE */				TEXT("Toggle Mute"),			&ToggleMute			},
	{ /* CMD_SONG_RATING_CLEAR */		TEXT("Clear Song Rating"),		&SongRatingClear	},
	{ /* CMD_SONG_RATING_1 */			TEXT("Set Rate 1 Star"),		&SongRating1		},
	{ /* CMD_SONG_RATING_2 */			TEXT("Set Rate 2 Stars"),		&SongRating2		},
	{ /* CMD_SONG_RATING_3 */			TEXT("Set Rate 3 Stars"),		&SongRating3		},
	{ /* CMD_SONG_RATING_4 */			TEXT("Set Rate 4 Stars"),		&SongRating4		},
	{ /* CMD_SONG_RATING_5 */			TEXT("Set Rate 5 Stars"),		&SongRating5		}
};
