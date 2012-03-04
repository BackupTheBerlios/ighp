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

#include "GlobalHotkeys.h"
#include "ConfigDialog.h"

void OpenConfigDialog()
{
	static ConfigDialog* pDialog = 0;

	if (pDialog && pDialog->IsWindow())
	{
		pDialog->ShowWindow();
		pDialog->BringWindowToTop();
		return;
	}

	pDialog = new ConfigDialog(Plugin::GetInstance()->GetMainWindow());
	pDialog->DoModal();
	SAFE_DEL(pDialog);
}

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
	{ /* CMD_OPEN_CONFIG_DIALOG */		IDS_CMD_OPEN_CONFIG_DIALOG,			&OpenConfigDialog	},
	{ /* CMD_PLAYPAUSE */				IDS_CMD_PLAYPAUSE,					&PlayPause			},
	{ /* CMD_STOP */					IDS_CMD_STOP,						&Stop				},
	{ /* CMD_NEXT_TRACK */				IDS_CMD_NEXT_TRACK,					&NextTrack			},
	{ /* CMD_PREVIOUS_TRACK */			IDS_CMD_PREVIOUS_TRACK,				&PreviousTrack		},
	{ /* CMD_TOGGLE_RANDOM */			IDS_CMD_TOGGLE_RANDOM,				&ToggleRandom		},
	{ /* CMD_TOGGLE_REPEAT */			IDS_CMD_TOGGLE_REPEAT,				&ToggleRepeat		},
	{ /* CMD_SHOWHIDE */				IDS_CMD_SHOWHIDE,					&ShowHide			},
	{ /* CMD_VOLUME_UP */				IDS_CMD_VOLUME_UP,					&VolumeUp			},
	{ /* CMD_VOLUME_DOWN */				IDS_CMD_VOLUME_DOWN,				&VolumeDown			},
	{ /* CMD_TOGGLE_MUTE */				IDS_CMD_TOGGLE_MUTE,				&ToggleMute			},
	{ /* CMD_SONG_RATING_CLEAR */		IDS_CMD_SONG_RATING_CLEAR,			&SongRatingClear	},
	{ /* CMD_SONG_RATING_1 */			IDS_CMD_SONG_RATING_1,				&SongRating1		},
	{ /* CMD_SONG_RATING_2 */			IDS_CMD_SONG_RATING_2,				&SongRating2		},
	{ /* CMD_SONG_RATING_3 */			IDS_CMD_SONG_RATING_3,				&SongRating3		},
	{ /* CMD_SONG_RATING_4 */			IDS_CMD_SONG_RATING_4,				&SongRating4		},
	{ /* CMD_SONG_RATING_5 */			IDS_CMD_SONG_RATING_5,				&SongRating5		}
};
