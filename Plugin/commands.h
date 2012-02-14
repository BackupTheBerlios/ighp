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

#pragma once

enum eCommand
{
	CMD_OPEN_CONFIG_DIALOG,
	CMD_PLAYPAUSE,
	CMD_STOP,
	CMD_NEXT_TRACK,
	CMD_PREVIOUS_TRACK,
	CMD_TOGGLE_RANDOM,
	CMD_TOGGLE_REPEAT,
	CMD_SHOWHIDE,
	CMD_VOLUME_UP,
	CMD_VOLUME_DOWN,
	CMD_TOGGLE_MUTE,
	CMD_SONG_RATING_CLEAR,
	CMD_SONG_RATING_1,
	CMD_SONG_RATING_2,
	CMD_SONG_RATING_3,
	CMD_SONG_RATING_4,
	CMD_SONG_RATING_5,
	CMD_COUNT
};

struct Command
{
	char_t* name;
	void (* execute)(void);
};

extern const Command COMMANDS [ CMD_COUNT ];
