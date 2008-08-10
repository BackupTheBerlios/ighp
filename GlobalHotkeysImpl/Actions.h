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
	eActionPreviousTrack
};

extern std::map<const std::string, Actions> actionsMap;

void InitActionsMap();

void PlayPause();
void NextTrack();
void PreviousTrack();

#endif /* ACTIONS_H */