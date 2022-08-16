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

#ifndef __GISH_GAME_CUSTOM_H__
#define __GISH_GAME_CUSTOM_H__

#include "sdl/file.h"

void custommenu(void);
void loadlevelmenu(void);
void playcampaignmenu(void);
void campaignmenu(void);
void loadplayermappack(void);
void saveplayermappack(void);
void loadmappack(const char *path);
void savemappack(void);
void newmappackmenu(void);

extern filelist_t levellist;

#define MAPPACK_FILENAME_LEN 32
#define MAPPACK_LEVEL_LEN 32
#define MAPPACK_LEVELNAME_LEN 32
struct MAPPACK
  {
  int active;
  int numoflevels;
  char filename[MAPPACK_FILENAME_LEN];
  char name[32];
  char level[64][MAPPACK_LEVEL_LEN];
  char levelname[64][MAPPACK_LEVELNAME_LEN];
  char levelmusic[64][32];
  char ending[16][64];
  };
extern struct MAPPACK mappack;

struct PLAYERMAPPACK
  {
  int levelnum;
  int numoflives;
  int totalscore;
  int difficulty;
  int highscore;
  };
extern struct PLAYERMAPPACK playermappack[16];

#endif
