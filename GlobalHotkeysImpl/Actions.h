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

#pragma once

#include "GhAction.h"

class OpenSettingsDialogAction : public GhAction
{
public:
	OpenSettingsDialogAction() : GhAction("Open Settings Dialog") {};
	void execute() const;
}; 

class PlayPauseAction : public GhAction
{
public:
	PlayPauseAction() : GhAction("Play/Pause") {};
	void execute() const;
};

class StopAction : public GhAction
{
public:
	StopAction() : GhAction("Stop") {};
	void execute() const;
};

class NextTrackAction : public GhAction
{
public:
	NextTrackAction() : GhAction("NextTrack") {};
	void execute() const;
};

class PreviousTrackAction : public GhAction
{
public:
	PreviousTrackAction() : GhAction("PreviousTrack") {};
	void execute() const;
};

class ToggleRandomAction : public GhAction
{
public:
	ToggleRandomAction() : GhAction("ToggleRandom") {};
	void execute() const;
};

class ToggleRepeatAction : public GhAction
{
public:
	ToggleRepeatAction() : GhAction("ToggleRepeat") {};
	void execute() const;
};

class ShowHideAction : public GhAction
{
public:
	ShowHideAction() : GhAction("ShowHide") {};
	void execute() const;
};

class ToggleVolumeAction 
{
protected:
	void ToggleVolume(const long step) const;
};

class VolumeUpAction : public GhAction, public ToggleVolumeAction
{
public:
	VolumeUpAction() : GhAction("VolumeUp") {};
	void execute() const;
};

class VolumeDownAction : public GhAction, public ToggleVolumeAction
{
public:
	VolumeDownAction() : GhAction("VolumeDown") {};
	void execute() const;
};

class ToggleMuteAction : public GhAction
{
public:
	ToggleMuteAction() : GhAction("ToggleMute") {};
	void execute() const;
};

class RateSongAction
{
protected:
	void RateSong(const unsigned int rating) const;
};

class SongRatingClearAction : public GhAction, public RateSongAction
{
public:
	SongRatingClearAction() : GhAction("SongRatingClear") {};
	void execute() const;
};

class SongRating1Action : public GhAction, public RateSongAction
{
public:
	SongRating1Action() : GhAction("SongRating1") {};
	void execute() const;
};

class SongRating2Action : public GhAction, public RateSongAction
{
public:
	SongRating2Action() : GhAction("SongRating2") {};
	void execute() const;
};

class SongRating3Action : public GhAction, public RateSongAction
{
public:
	SongRating3Action() : GhAction("SongRating3") {};
	void execute() const;
};

class SongRating4Action : public GhAction, public RateSongAction
{
public:
	SongRating4Action() : GhAction("SongRating4") {};
	void execute() const;
};

class SongRating5Action : public GhAction, public RateSongAction
{
public:
	SongRating5Action() : GhAction("SongRating5") {};
	void execute() const;
};

//class Action : public GhAction
//{
//public:
//	Action() : GhAction("") {};
//	void execute() const;
//};
