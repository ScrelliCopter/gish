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

#include "keyboard.h"

#include <SDL3/SDL.h>
#include <string.h>

bool keyboard[SDL_SCANCODE_COUNT];
bool prevkeyboard[SDL_SCANCODE_COUNT];

void checkkeyboard(void)
  {
  memcpy(prevkeyboard,keyboard,SDL_SCANCODE_COUNT);
  memcpy(keyboard,SDL_GetKeyboardState(NULL),SDL_SCANCODE_COUNT);

	if (keyboard[SDL_SCANCODE_KP_ENTER])
		keyboard[SDL_SCANCODE_RETURN]=1;
	if (keyboard[SDL_SCANCODE_RCTRL])
		keyboard[SDL_SCANCODE_LCTRL]=1;
	if (keyboard[SDL_SCANCODE_RSHIFT])
		keyboard[SDL_SCANCODE_LSHIFT]=1;

  if (keyboard[SCAN_NUMLEFT])
    keyboard[SCAN_LEFT]=1;
  if (keyboard[SCAN_NUMRIGHT])
    keyboard[SCAN_RIGHT]=1;
  if (keyboard[SCAN_NUMUP])
    keyboard[SCAN_UP]=1;
  if (keyboard[SCAN_NUMDOWN])
    keyboard[SCAN_DOWN]=1;
  }

