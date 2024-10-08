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

#ifndef __GISH_INPUT_KEYBOARD_H__
#define __GISH_INPUT_KEYBOARD_H__

#include <SDL3/SDL_scancode.h>

void checkkeyboard(void);

extern bool keyboard[SDL_SCANCODE_COUNT];
extern bool prevkeyboard[SDL_SCANCODE_COUNT];

#define SCAN_ESC          SDL_SCANCODE_ESCAPE
#define SCAN_0            SDL_SCANCODE_0
#define SCAN_1            SDL_SCANCODE_1
#define SCAN_2            SDL_SCANCODE_2
#define SCAN_3            SDL_SCANCODE_3
#define SCAN_4            SDL_SCANCODE_4
#define SCAN_5            SDL_SCANCODE_5
#define SCAN_6            SDL_SCANCODE_6
#define SCAN_7            SDL_SCANCODE_7
#define SCAN_8            SDL_SCANCODE_8
#define SCAN_9            SDL_SCANCODE_9
#define SCAN_MINUS        SDL_SCANCODE_MINUS
#define SCAN_EQUALS       SDL_SCANCODE_EQUALS
#define SCAN_BKSP         SDL_SCANCODE_BACKSPACE
#define SCAN_TAB          SDL_SCANCODE_TAB
#define SCAN_LFT_BRACKET  SDL_SCANCODE_LEFTBRACKET
#define SCAN_RGT_BRACKET  SDL_SCANCODE_RIGHTBRACKET
#define SCAN_ENTER        SDL_SCANCODE_RETURN
#define SCAN_CTRL         SDL_SCANCODE_LCTRL
#define SCAN_RCTRL        SDL_SCANCODE_RCTRL
#define SCAN_SEMI         SDL_SCANCODE_SEMICOLON
#define SCAN_APOS         SDL_SCANCODE_APOSTROPHE
#define SCAN_TILDE        SDL_SCANCODE_GRAVE
#define SCAN_SHIFT        SDL_SCANCODE_LSHIFT
#define SCAN_BACK_SLASH   SDL_SCANCODE_BACKSLASH
#define SCAN_COMMA        SDL_SCANCODE_COMMA
#define SCAN_PERIOD       SDL_SCANCODE_PERIOD
#define SCAN_FOWARD_SLASH SDL_SCANCODE_SLASH
#define SCAN_RSHIFT       SDL_SCANCODE_RSHIFT
#define SCAN_PRT_SCRN     SDL_SCANCODE_PRINTSCREEN
#define SCAN_ALT          SDL_SCANCODE_LALT
#define SCAN_SPACE        SDL_SCANCODE_SPACE
#define SCAN_CAPS_LOCK    SDL_SCANCODE_CAPSLOCK
#define SCAN_F1           SDL_SCANCODE_F1
#define SCAN_F2           SDL_SCANCODE_F2
#define SCAN_F3           SDL_SCANCODE_F3
#define SCAN_F4           SDL_SCANCODE_F4
#define SCAN_F5           SDL_SCANCODE_F5
#define SCAN_F6           SDL_SCANCODE_F6
#define SCAN_F7           SDL_SCANCODE_F7
#define SCAN_F8           SDL_SCANCODE_F8
#define SCAN_F9           SDL_SCANCODE_F9
#define SCAN_F10          SDL_SCANCODE_F10
#define SCAN_F11          SDL_SCANCODE_F11
#define SCAN_F12          SDL_SCANCODE_F12
#define SCAN_NUM_LOCK     SDL_SCANCODE_NUMLOCKCLEAR
#define SCAN_SCROLL_LOCK  SDL_SCANCODE_SCROLLLOCK
#define SCAN_NUMHOME      SDL_SCANCODE_KP_7
#define SCAN_UP           SDL_SCANCODE_UP
#define SCAN_NUMPGUP      SDL_SCANCODE_KP_9
#define SCAN_NUM_MINUS    SDL_SCANCODE_KP_MINUS
#define SCAN_LEFT         SDL_SCANCODE_LEFT
#define SCAN_CENTER       SDL_SCANCODE_KP_5
#define SCAN_RIGHT        SDL_SCANCODE_RIGHT
#define SCAN_NUMPLUS      SDL_SCANCODE_KP_PLUS
#define SCAN_NUMEND       SDL_SCANCODE_KP_1
#define SCAN_DOWN         SDL_SCANCODE_DOWN
#define SCAN_NUMPGDWN     SDL_SCANCODE_KP_3
#define SCAN_NUMINS       SDL_SCANCODE_KP_0
#define SCAN_NUMDEL       SDL_SCANCODE_KP_PERIOD
#define SCAN_LWIN         SDL_SCANCODE_LWIN
#define SCAN_RWIN         SDL_SCANCODE_RWIN
#define SCAN_APPS         SDL_SCANCODE_APPS
#define SCAN_NUMENTER     SDL_SCANCODE_KP_ENTER
#define SCAN_NUMLEFT      SDL_SCANCODE_KP_4
#define SCAN_NUMRIGHT     SDL_SCANCODE_KP_6
#define SCAN_NUMUP        SDL_SCANCODE_KP_8
#define SCAN_NUMDOWN      SDL_SCANCODE_KP_2
#define SCAN_LEFTARROW    SDL_SCANCODE_LEFT
#define SCAN_RIGHTARROW   SDL_SCANCODE_RIGHT
#define SCAN_UPARROW      SDL_SCANCODE_UP
#define SCAN_DOWNARROW    SDL_SCANCODE_DOWN
#define SCAN_INSERT       SDL_SCANCODE_INSERT
#define SCAN_DELETE       SDL_SCANCODE_DELETE
#define SCAN_HOME         SDL_SCANCODE_HOME
#define SCAN_END          SDL_SCANCODE_END
#define SCAN_PAGEUP       SDL_SCANCODE_PAGEUP
#define SCAN_PAGEDOWN     SDL_SCANCODE_PAGEDOWN
#define SCAN_A            SDL_SCANCODE_A
#define SCAN_B            SDL_SCANCODE_B
#define SCAN_C            SDL_SCANCODE_C
#define SCAN_D            SDL_SCANCODE_D
#define SCAN_E            SDL_SCANCODE_E
#define SCAN_F            SDL_SCANCODE_F
#define SCAN_G            SDL_SCANCODE_G
#define SCAN_H            SDL_SCANCODE_H
#define SCAN_I            SDL_SCANCODE_I
#define SCAN_J            SDL_SCANCODE_J
#define SCAN_K            SDL_SCANCODE_K
#define SCAN_L            SDL_SCANCODE_L
#define SCAN_M            SDL_SCANCODE_M
#define SCAN_N            SDL_SCANCODE_N
#define SCAN_O            SDL_SCANCODE_O
#define SCAN_P            SDL_SCANCODE_P
#define SCAN_Q            SDL_SCANCODE_Q
#define SCAN_R            SDL_SCANCODE_R
#define SCAN_S            SDL_SCANCODE_S
#define SCAN_T            SDL_SCANCODE_T
#define SCAN_U            SDL_SCANCODE_U
#define SCAN_V            SDL_SCANCODE_V
#define SCAN_W            SDL_SCANCODE_W
#define SCAN_X            SDL_SCANCODE_X
#define SCAN_Y            SDL_SCANCODE_Y
#define SCAN_Z            SDL_SCANCODE_Z

#endif
