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

#include "GlobalHotkeys.h"
#include "GhAction.h"

class OpenSettingsDialogAction : public GhAction
{
public:
	OpenSettingsDialogAction() : GhAction("Open Settings Dialog") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(OpenSettingsDialogAction);
}; 

class PlayPauseAction : public GhAction
{
public:
	PlayPauseAction() : GhAction("Play/Pause") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(PlayPauseAction);
};

class StopAction : public GhAction
{
public:
	StopAction() : GhAction("Stop") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(StopAction);
};

class NextTrackAction : public GhAction
{
public:
	NextTrackAction() : GhAction("Next Track") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(NextTrackAction);
};

class PreviousTrackAction : public GhAction
{
public:
	PreviousTrackAction() : GhAction("Previous Track") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(PreviousTrackAction);
};

class ToggleRandomAction : public GhAction
{
public:
	ToggleRandomAction() : GhAction("Toggle Random") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(ToggleRandomAction);
};

class ToggleRepeatAction : public GhAction
{
public:
	ToggleRepeatAction() : GhAction("Toggle Repeat") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(ToggleRepeatAction);
};

class ShowHideAction : public GhAction
{
public:
	ShowHideAction() : GhAction("Show/Hide") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(ShowHideAction);
};

class ToggleVolumeAction : public GhAction
{
protected:
	explicit ToggleVolumeAction(QString name) : GhAction(name) {}
	void ToggleVolume(const long step) const;

private:
	DISALLOW_COPY_AND_ASSIGN(ToggleVolumeAction);
};

class VolumeUpAction : public ToggleVolumeAction
{
public:
	VolumeUpAction() : ToggleVolumeAction("Increase Volume") {}
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(VolumeUpAction);
};

class VolumeDownAction : public ToggleVolumeAction
{
public:
	VolumeDownAction() : ToggleVolumeAction("Decrease Volume") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(VolumeDownAction);
};

class ToggleMuteAction : public GhAction
{
public:
	ToggleMuteAction() : GhAction("Toggle Mute") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(ToggleMuteAction);
};

class RateSongAction : public GhAction
{
protected:
	explicit RateSongAction(QString name) : GhAction(name) {}
	void RateSong(const unsigned int rating) const;

private:
	DISALLOW_COPY_AND_ASSIGN(RateSongAction);
};

class SongRatingClearAction : public RateSongAction
{
public:
	SongRatingClearAction() : RateSongAction("Clear Song Rating") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(SongRatingClearAction);
};

class SongRating1Action : public RateSongAction
{
public:
	SongRating1Action() : RateSongAction("Set Rate 1 Star") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(SongRating1Action);
};

class SongRating2Action : public RateSongAction
{
public:
	SongRating2Action() : RateSongAction("Set Rate 2 Stars") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(SongRating2Action);
};

class SongRating3Action : public RateSongAction
{
public:
	SongRating3Action() : RateSongAction("Set Rate 3 Stars") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(SongRating3Action);
};

class SongRating4Action : public RateSongAction
{
public:
	SongRating4Action() : RateSongAction("Set Rate 4 Stars") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(SongRating4Action);
};

class SongRating5Action : public RateSongAction
{
public:
	SongRating5Action() : RateSongAction("Set Rate 5 Stars") {};
	void execute() const;

private:
	DISALLOW_COPY_AND_ASSIGN(SongRating5Action);
};

//class Action : public GhAction
//{
//public:
//	Action() : GhAction("") {};
//	void execute() const;
//
//private:
//	DISALLOW_COPY_AND_ASSIGN();
//};
