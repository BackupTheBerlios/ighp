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