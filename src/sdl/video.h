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

#ifndef __GISH_SDL_VIDEO_H__
#define __GISH_SDL_VIDEO_H__

#include <SDL_video.h>

void createwindow(void);
void getvideoinfo(void);
void listvideomodes(void);

extern int numofsdldisplays;
extern SDL_Rect sdldisplaybounds[64];
extern int numofsdlvideomodes;
struct SDLVIDEOMODE
  {
  int displayid;
  SDL_DisplayMode displaymode;
  int bitsperpixel;
  };
extern struct SDLVIDEOMODE sdlvideomode[4096];

extern SDL_Window *sdlwindow;
extern SDL_GLContext *sdlglcontext;

#endif
