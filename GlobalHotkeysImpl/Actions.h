#ifndef ACTIONS_H
#define ACTIONS_H

#include <windows.h>
#include <comutil.h>

#include <string>
#include <map>

#include "iTunesCOMInterface.h"

static enum Actions
{
	eActionNotDefined,
	eActionPlayPause,
	eActionNextTrack,
	eActionPreviousTrack,
	eActionRandom,
	eActionRepeat,
	eActionSongRatingClear,
	eActionSongRating1,
	eActionSongRating2,
	eActionSongRating3,
	eActionSongRating4,
	eActionSongRating5
};

extern std::map<const std::string, Actions> actionsMap;

void InitActionsMap();

void PlayPause();
void NextTrack();
void PreviousTrack();
void Random();
void Repeat();
void ClearSongRating();
void SongRating1();
void SongRating2();
void SongRating3();
void SongRating4();
void SongRating5();

#endif /* ACTIONS_H */