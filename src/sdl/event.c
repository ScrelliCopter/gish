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

#include "event.h"

#include <SDL3/SDL.h>
#include "video/texture.h"
#include "game/game.h"

struct WINDOWINFO windowinfo;

void checksystemmessages(void)
{
  int count;
  SDL_Event event;

  if (windowinfo.minimized)
    SDL_Delay(20);

  while (SDL_PollEvent(&event))
  {
    if (event.type==SDL_EVENT_WINDOW_FOCUS_GAINED)
    {
      windowinfo.minimized=0;
    }
    else if (event.type==SDL_EVENT_WINDOW_FOCUS_LOST)
    {
      if(game.exit==0)
        game.pause=1;
      windowinfo.minimized=1;
    }
    else if (event.type==SDL_EVENT_QUIT)
      windowinfo.shutdown=1;
  }
}
