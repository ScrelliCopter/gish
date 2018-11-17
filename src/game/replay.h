/*
Copyright (C) 2005, 2010 - Cryptic Sea

This file is part of Gish.

Gish is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef __GISH_GAME_REPLAY_H__
#define __GISH_GAME_REPLAY_H__

void saveinputs(void);
void loadinputs(void);
void savereplay(int levelnum);
int loadreplay(char *filename);
void replaymenu(void);

extern int numofreplayframes;
struct REPLAYFRAME
  {
  unsigned char button;
  };
extern struct REPLAYFRAME replayframe[65536];

#endif
