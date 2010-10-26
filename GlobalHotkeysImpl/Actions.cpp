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

#include "actions.h"
#include "GhDialog.h"

#include <windows.h>
#include <comutil.h>

#include "iTunesCOMInterface.h"

#include <QString>
#include <QTextStream>
#include <QMessageBox>

bool GhAction::operator==(const GhAction &other) const
{
	return mName == other.mName;
}

void GhAction::ShowErrorMessage() const
{
	QString errorMessage;
	QTextStream(&errorMessage) << "Could not execute action: " << mName 
		<< ". iTunes is not responding.";

	QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setWindowTitle("Global Hotkeys Plugin");
	msgBox.setText(errorMessage);
	msgBox.exec();
}

const QList<GhAction*> GhAction::Actions = QList<GhAction*>()
	<< new OpenSettingsDialogAction()
	<< new PlayPauseAction()
	<< new StopAction()
	<< new NextTrackAction()
	<< new PreviousTrackAction()
	<< new ToggleRandomAction()
	<< new ToggleRepeatAction()
	<< new ShowHideAction()
	<< new VolumeUpAction()
	<< new VolumeDownAction()
	<< new ToggleMuteAction()
	<< new SongRatingClearAction()
	<< new SongRating1Action()
	<< new SongRating2Action()
	<< new SongRating3Action()
	<< new SongRating4Action()
	<< new SongRating5Action();

void OpenSettingsDialogAction::execute() const
{
	GhDialog dialog = GhDialog(0);
	dialog.exec();
}

void PlayPauseAction::execute() const 
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->PlayPause();
		iITunes->Release();
	} else {
		ShowErrorMessage();
	}

	CoUninitialize();
}

void StopAction::execute() const
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->Stop();
		iITunes->Release();
	} else {
		ShowErrorMessage();
	}

	CoUninitialize();
}

void NextTrackAction::execute() const
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->NextTrack();
		iITunes->Release();
	} else {
		ShowErrorMessage();
	}

	CoUninitialize();
}

void PreviousTrackAction::execute() const
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->PreviousTrack();
		iITunes->Release();
	} else {
		ShowErrorMessage();
	}

	CoUninitialize();
}

void ToggleRandomAction::execute() const
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
	} else {
		ShowErrorMessage();
	}

	CoUninitialize();
}

void ToggleRepeatAction::execute() const
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
	} else {
		ShowErrorMessage();
	}

	CoUninitialize();
}

void ShowHideAction::execute() const
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
	} else {
		ShowErrorMessage();
	}

	CoUninitialize();
}

void ToggleVolumeAction::ToggleVolume(const long step) const
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		long volume = 50;
		iITunes->get_SoundVolume(&volume);

		volume += step;
		iITunes->put_SoundVolume(volume);

		iITunes->Release();
	}

	CoUninitialize();
}

void VolumeUpAction::execute() const
{
	ToggleVolume(5);
}

void VolumeDownAction::execute() const
{
	ToggleVolume(-5);
}

void ToggleMuteAction::execute() const
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		VARIANT_BOOL isMuted = 0;
		iITunes->get_Mute(&isMuted);
		
		isMuted = (isMuted == 0) ? -1 : 0;
		iITunes->put_Mute(isMuted);

		iITunes->Release();
	} else {
		ShowErrorMessage();
	}

	CoUninitialize();
}

void RateSongAction::RateSong(const unsigned int rating) const
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

void SongRatingClearAction::execute() const
{
	RateSong(0);
}

void SongRating1Action::execute() const
{
	RateSong(20);
}

void SongRating2Action::execute() const
{
	RateSong(40);
}

void SongRating3Action::execute() const
{
	RateSong(60);
}

void SongRating4Action::execute() const
{
	RateSong(80);
}

void SongRating5Action::execute() const
{
	RateSong(100);
}

//void Action::execute() const
//{
//
//}