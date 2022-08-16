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

#include "options.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include "glext.h"
#include "sdl/event.h"
#include "sdl/video.h"
#include "video/text.h"
#include "video/texture.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/joystick.h"
#include "menu/menu.h"
#include "english.h"

struct OPTION option;
struct CONTROL control[8];

void optionsmenu(void)
  {
  int count,count2,count3;

  count=0;

  joystickmenu=0;

  resetmenuitems();

  while (!menuitem[0].active && !windowinfo.shutdown)
    {
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    numofmenuitems=0;
    createmenuitem(TXT_BACK,0,0,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_ESC);
    count=144;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;

    count=336;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;

    count=144;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;

    count=336;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;

    createmenuitem(TXT_VIDEOOPTIONS,320,0,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_V);
    createmenuitem(TXT_4_PLAYER,160,0,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_4);

    count=112;
    if (control[0].joysticknum==-1)
      {
      createmenuitem(TXT_NONE,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[0].joysticknum,0);
      }
    if (control[0].joysticknum==0)
      {
      createmenuitem(TXT_JOY1,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[0].joysticknum,1);
      }
    if (control[0].joysticknum==1)
      {
      createmenuitem(TXT_JOY2,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[0].joysticknum,2);
      }
    if (control[0].joysticknum==2)
      {
      createmenuitem(TXT_JOY3,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[0].joysticknum,3);
      }
    if (control[0].joysticknum==3)
      {
      createmenuitem(TXT_JOY4,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[0].joysticknum,-1);
      }
    count=304;
    if (control[1].joysticknum==-1)
      {
      createmenuitem(TXT_NONE,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[1].joysticknum,0);
      }
    if (control[1].joysticknum==0)
      {
      createmenuitem(TXT_JOY1,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[1].joysticknum,1);
      }
    if (control[1].joysticknum==1)
      {
      createmenuitem(TXT_JOY2,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[1].joysticknum,2);
      }
    if (control[1].joysticknum==2)
      {
      createmenuitem(TXT_JOY3,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[1].joysticknum,3);
      }
    if (control[1].joysticknum==3)
      {
      createmenuitem(TXT_JOY4,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[1].joysticknum,-1);
      }

    if (option.sound)
      {
      createmenuitem(TXT_SOUND_ON,0,32,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&option.sound,0);
      }
    else
      {
      createmenuitem(TXT_SOUND_OFF,0,32,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&option.sound,1);
      }
    if (option.music)
      {
      createmenuitem(TXT_MUSIC_ON,0,48,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&option.music,0);
      }
    else
      {
      createmenuitem(TXT_MUSIC_OFF,0,48,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&option.music,1);
      }

    checksystemmessages();
    checkkeyboard();
    checkmouse();
    checkjoystick();
    checkmenuitems();

    if (control[0].joysticknum>=numofjoysticks)
      control[0].joysticknum=-1;
    if (control[1].joysticknum>=numofjoysticks)
      control[1].joysticknum=-1;

    setuptextdisplay();

    drawtext(TXT_PLAYER,0,80,16,1.0f,1.0f,1.0f,1.0f);
    drawtext(TXT_KEY,320,80,16,1.0f,1.0f,1.0f,1.0f);
    drawtext(TXT_JOYSTICK,480,80,16,1.0f,1.0f,1.0f,1.0f);

    drawtext(TXT_PLAYER1,0,112,16,1.0f,1.0f,1.0f,1.0f);

    count2=0;
    count=0;
    drawtext(TXT_MOVE_LEFT,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[0]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_RIGHT,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[1]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_DOWN,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[2]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_UP,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[3]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_STICK,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[4]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_JUMP,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[5]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_SLIDE,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[6]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_HEAVY,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[7]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_START_PAUSE,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_PLAYER2,0,304,16,1.0f,1.0f,1.0f,1.0f);

    count2=1;
    count=0;
    drawtext(TXT_MOVE_LEFT,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[0]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_RIGHT,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[1]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_DOWN,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[2]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_UP,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[3]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_STICK,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[4]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_JUMP,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[5]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_SLIDE,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[6]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_HEAVY,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[7]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    if (control[0].joysticknum!=-1)
      drawtext(joystick[control[0].joysticknum].name,480,128,10,0.5f,0.5f,0.5f,1.0f);
    if (control[1].joysticknum!=-1)
      drawtext(joystick[control[1].joysticknum].name,480,320,10,0.5f,0.5f,0.5f,1.0f);

    drawmenuitems();

    drawsliderbars();

    drawmousecursor(768+font.cursornum,mouse.x,mouse.y,16,1.0f,1.0f,1.0f,1.0f);

    SDL_GL_SwapWindow(sdlwindow);

    for (count=0;count<8;count++)
    if (menuitem[count+1].active)
      {
      for (count2=1;count2<323;count2++)
      if (keyboardlabel[count2][0]!=0)
      if (keyboard[count2] && !prevkeyboard[count2])
        {
        control[0].key[count]=count2;

        for (count3=0;count3<8;count3++)
        if (count3!=count)
        if (control[0].key[count3]==count2)
          control[0].key[count3]=0;

        for (count3=0;count3<8;count3++)
        if (count3!=count)
        if (control[1].key[count3]==count2)
          control[1].key[count3]=0;

        menuitem[count+1].active=0;
        }
      if (keyboard[SCAN_DELETE] && !prevkeyboard[SCAN_DELETE])
        {
        control[0].key[count]=0;
        menuitem[count+1].active=0;
        }
      }
    for (count=0;count<8;count++)
    if (menuitem[count+9].active)
      {
      for (count2=1;count2<323;count2++)
      if (keyboardlabel[count2][0]!=0)
      if (keyboard[count2] && !prevkeyboard[count2])
        {
        control[1].key[count]=count2;

        for (count3=0;count3<8;count3++)
        if (count3!=count)
        if (control[0].key[count3]==count2)
          control[0].key[count3]=0;

        for (count3=0;count3<8;count3++)
        if (count3!=count)
        if (control[1].key[count3]==count2)
          control[1].key[count3]=0;

        menuitem[count+9].active=0;
        }
      if (keyboard[SCAN_DELETE] && !prevkeyboard[SCAN_DELETE])
        {
        control[1].key[count]=0;
        menuitem[count+9].active=0;
        }
      }
    if (control[0].joysticknum!=-1)
    for (count=0;count<9;count++)
    if (menuitem[count+17].active)
      {
      for (count2=0;count2<joystick[control[0].joysticknum].numofbuttons;count2++)
      if (joystick[control[0].joysticknum].button[count2] && !prevjoystick[control[0].joysticknum].button[count2])
        {
        control[0].button[count]=count2;

        if (control[0].joysticknum!=-1)
        for (count3=0;count3<joystick[control[0].joysticknum].numofbuttons;count3++)
        if (count3!=count)
        if (control[0].button[count3]==count2)
          control[0].button[count3]=-1;
        /*
        if (control[1].joysticknum!=-1)
        for (count3=0;count3<joystick[control[1].joysticknum].numofbuttons;count3++)
        if (count3!=count)
        if (control[1].button[count3]==count2)
          control[1].button[count3]=-1;
        */
        menuitem[count+17].active=0;
        }
      if (keyboard[SCAN_DELETE] && !prevkeyboard[SCAN_DELETE])
        {
        control[0].button[count]=-1;
        menuitem[count+17].active=0;
        }
      }
    if (control[1].joysticknum!=-1)
    for (count=0;count<8;count++)
    if (menuitem[count+26].active)
      {
      for (count2=0;count2<joystick[control[1].joysticknum].numofbuttons;count2++)
      if (joystick[control[1].joysticknum].button[count2] && !prevjoystick[control[1].joysticknum].button[count2])
        {
        control[1].button[count]=count2;
        /*
        if (control[0].joysticknum!=-1)
        for (count3=0;count3<8;count3++)
        if (count3!=count)
        if (control[0].button[count3]==count2)
          control[0].button[count3]=-1;
        */
        if (control[1].joysticknum!=-1)
        for (count3=0;count3<8;count3++)
        if (count3!=count)
        if (control[1].button[count3]==count2)
          control[1].button[count3]=-1;

        menuitem[count+26].active=0;
        }
      if (keyboard[SCAN_DELETE] && !prevkeyboard[SCAN_DELETE])
        {
        control[1].button[count]=-1;
        menuitem[count+26].active=0;
        }
      }
    if (menuitem[34].active)
      videooptionsmenu();
    if (menuitem[35].active)
      optionsmenu2();

    if (mouse.lmb)
    if (mouse.x>=160 && mouse.x<288)
      {
      if (mouse.y>=32 && mouse.y<48)
        option.soundvolume=(float)(mouse.x-160)/128.0f;
      if (mouse.y>=48 && mouse.y<64)
        option.musicvolume=(float)(mouse.x-160)/128.0f;
      }
    }

  resetmenuitems();

  joystickmenu=1;
  }


#define PAGENUMVIDMODES 20

void videooptionsmenu(void)
  {
  int ypos;
  int pagenum,numpages;
  int modecount;
  int prevvideomodenum;
  int videomodenum;
  char restext[64];
  int fullscreen;
  int bitsperpixel;
  int refreshrate;
  int displayid,prevdisplayid;
  char *glvendor;
  char *glrenderer;
  char *glversion;

  glvendor=(char *) glGetString(GL_VENDOR);
  glrenderer=(char *) glGetString(GL_RENDERER);
  glversion=(char *) glGetString(GL_VERSION);

  pagenum=0;
  modecount=0;
  videomodenum=-1;
  for (int i=0;i<numofsdlvideomodes;i++)
    {
    if (windowinfo.displayid!=sdlvideomode[i].displayid)
      continue;
    if (sdlvideomode[i].displaymode.w<640)
      continue;

    modecount++;

    if (windowinfo.resolutionx!=sdlvideomode[i].displaymode.w)
      continue;
    if (windowinfo.resolutiony!=sdlvideomode[i].displaymode.h)
      continue;
    if (windowinfo.bitsperpixel!=sdlvideomode[i].bitsperpixel)
      continue;
    if (windowinfo.refreshrate!=sdlvideomode[i].displaymode.refresh_rate)
      continue;

    videomodenum=i;
    pagenum=(modecount-1)/PAGENUMVIDMODES;
    break;
    }
  prevvideomodenum=videomodenum;
  fullscreen=windowinfo.fullscreen;
  bitsperpixel=windowinfo.bitsperpixel;
  refreshrate=windowinfo.refreshrate;
  prevdisplayid=displayid=windowinfo.displayid;

  resetmenuitems();
  numpages=0;

  while (!menuitem[0].active && !menuitem[1].active && !windowinfo.shutdown)
    {
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    numofmenuitems=0;
    createmenuitem(TXT_BACK,0,0,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_ESC);
    createmenuitem(TXT_APPLY,(640|TEXT_END),0,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_A);

    ypos=32;
    drawtext(TXT_RESOLUTION,0,ypos,16,1.0f,1.0f,1.0f,1.0f);
    ypos+=16;
    modecount=0;
    for (int i=0;i<numofsdlvideomodes;i++)
      {
      if (sdlvideomode[i].displaymode.w<640)
        continue;
      if (sdlvideomode[i].displayid!=displayid)
        continue;
      if (fullscreen==2)
        {
        if (sdlvideomode[i].displaymode.w!=sdldisplay[sdlvideomode[i].displayid].bounds.w)
          continue;
        if (sdlvideomode[i].displaymode.h!=sdldisplay[sdlvideomode[i].displayid].bounds.h)
          continue;
        }

      modecount++;

      if (modecount-1<pagenum*PAGENUMVIDMODES)
        continue;
      if (modecount-1>=pagenum*PAGENUMVIDMODES+PAGENUMVIDMODES)
        continue;

      struct SDLVIDEOMODE *vidmode=&sdlvideomode[i];
      int pad=0;

      pad+=(vidmode->displaymode.w>=1000)?0:1;
      pad+=(vidmode->displaymode.h>=1000)?0:1;
      pad+=(vidmode->bitsperpixel<10)?0:1;
      snprintf(restext,sizeof(restext),"%dx%d %*dbpp %dhz",
        vidmode->displaymode.w,
        vidmode->displaymode.h,
        pad+1,vidmode->bitsperpixel,
        vidmode->displaymode.refresh_rate);
      createmenuitem(restext,0,ypos,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&videomodenum,i);

      ypos+=16;
      }
    numpages=(modecount-1)/PAGENUMVIDMODES;

    ypos=48+PAGENUMVIDMODES*16+4;
    createmenuitem(TXT_PAGE_UP,0,ypos,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_PAGEUP);
    setmenuitem(MO_SET,&pagenum,pagenum-1);
    if (pagenum<=0)
      setmenuitem(MO_HIGHLIGHT,0);

    createmenuitem(TXT_PAGE_DOWN,(20*16)|TEXT_END,ypos,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_PAGEDOWN);
    setmenuitem(MO_SET,&pagenum,pagenum+1);
    if (pagenum>=numpages)
      setmenuitem(MO_HIGHLIGHT,0);

    ypos+=2;
    const float pagerluma=(numpages>0)?0.4f:0.2f;
    drawtext("/i///i",144|TEXT_CENTER,ypos,12,pagerluma,pagerluma,pagerluma,1.0f,pagenum+1,numpages+1);

    ypos=32;
    createmenuitem(TXT_WINDOWED,340,ypos,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_SET,&fullscreen,0);
    ypos+=16;
    createmenuitem(TXT_FULLSCREEN,340,ypos,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_SET,&fullscreen,1);
    ypos+=16;
    createmenuitem(TXT_BORDERLESS,340,ypos,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_SET,&fullscreen,2);
    ypos+=16*2;

    drawtext(TXT_DISPLAY,340,ypos,16,1.0f,1.0f,1.0f,1.0f);
    ypos+=16;

    for (int i=0;i<numofsdldisplays;i++)
      {
      createmenuitem(sdldisplay[i].name,340,ypos,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&displayid,i);
      ypos+=16;
      }
    if (prevdisplayid!=displayid)
      {
      pagenum=0;
      prevdisplayid=displayid;
      }

    checksystemmessages();
    checkkeyboard();
    checkmouse();
    checkjoystick();
    checkmenuitems();

    setuptextdisplay();

    ypos=400;
    drawtext(TXT_OPENGLINFO,0,ypos,16,1.0f,1.0f,1.0f,1.0f);
    ypos+=16;
    if (glext.multitexture)
      drawtext("GL_ARB_multitexture",0,ypos,12,0.0f,1.0f,0.0f,1.0f);
    else
      drawtext("GL_ARB_multitexture",0,ypos,12,0.25f,0.25f,0.25f,1.0f);
    ypos+=12;
    if (glext.texture_env_dot3)
      drawtext("GL_ARB_texture_env_dot3",0,ypos,12,0.0f,1.0f,0.0f,1.0f);
    else
      drawtext("GL_ARB_texture_env_dot3",0,ypos,12,0.25f,0.25f,0.25f,1.0f);
    ypos+=12;
    drawtext("GL_VERSION: /s",0,ypos,12,1.0f,1.0f,1.0f,1.0f,glversion);
    ypos+=12;
    drawtext("GL_VENDOR: /s",0,ypos,12,1.0f,1.0f,1.0f,1.0f,glvendor);
    ypos+=12;
    drawtext("GL_RENDERER: /s",0,ypos,12,1.0f,1.0f,1.0f,1.0f,glrenderer);

    drawmenuitems();

    drawmousecursor(768+font.cursornum,mouse.x,mouse.y,16,1.0f,1.0f,1.0f,1.0f);

    SDL_GL_SwapWindow(sdlwindow);
    }

  if (menuitem[1].active)
    {
    if (prevvideomodenum==videomodenum)
    if (windowinfo.fullscreen==fullscreen)
    if (windowinfo.bitsperpixel==bitsperpixel)
    if (windowinfo.refreshrate==refreshrate)
      return;

    if (videomodenum==-1)
      return;

    windowinfo.displayid=sdlvideomode[videomodenum].displayid;
    windowinfo.resolutionx=sdlvideomode[videomodenum].displaymode.w;
    windowinfo.resolutiony=sdlvideomode[videomodenum].displaymode.h;
    windowinfo.fullscreen=fullscreen;
    windowinfo.bitsperpixel=sdlvideomode[videomodenum].bitsperpixel;
    windowinfo.refreshrate=sdlvideomode[videomodenum].displaymode.refresh_rate;
    createwindow();

    for (int i=0;i<2048;i++)
      if (texture[i].sizex!=0)
        setuptexture(i);
    }

  resetmenuitems();
  }

void drawsliderbars(void)
  {
  float vec[3];

  glDisable(GL_TEXTURE_2D);

  glBegin(GL_QUADS);

  if (option.sound)
    {
    glColor4f(0.25f,0.25f,0.25f,1.0f);

    vec[0]=160.0f;
    vec[1]=40.0f-1.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f+128.0f;
    vec[1]=40.0f-5.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f+128.0f;
    vec[1]=40.0f+5.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f;
    vec[1]=40.0f+1.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    glColor4f(0.75f,0.75f,0.75f,1.0f);

    vec[0]=160.0f+option.soundvolume*128.0f-2.0f;
    vec[1]=40.0f-7.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f+option.soundvolume*128.0f+2.0f;
    vec[1]=40.0f-7.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f+option.soundvolume*128.0f+2.0f;
    vec[1]=40.0f+7.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f+option.soundvolume*128.0f-2.0f;
    vec[1]=40.0f+7.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);
    }
  if (option.music)
    {
    glColor4f(0.25f,0.25f,0.25f,1.0f);

    vec[0]=160.0f;
    vec[1]=56.0f-1.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f+128.0f;
    vec[1]=56.0f-5.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f+128.0f;
    vec[1]=56.0f+5.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f;
    vec[1]=56.0f+1.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    glColor4f(0.75f,0.75f,0.75f,1.0f);

    vec[0]=160.0f+option.musicvolume*128.0f-2.0f;
    vec[1]=56.0f-7.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f+option.musicvolume*128.0f+2.0f;
    vec[1]=56.0f-7.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f+option.musicvolume*128.0f+2.0f;
    vec[1]=56.0f+7.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);

    vec[0]=160.0f+option.musicvolume*128.0f-2.0f;
    vec[1]=56.0f+7.0f;
    convertscreenvertex(vec,font.sizex,font.sizey);
    glVertex3fv(vec);
    }

  glEnd();

  glEnable(GL_TEXTURE_2D);
  }

void optionsmenu2(void)
  {
  int count,count2,count3;

  count=0;

  resetmenuitems();

  while (!menuitem[0].active && !windowinfo.shutdown)
    {
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    numofmenuitems=0;
    createmenuitem(TXT_BACK,0,0,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_ESC);
    count=144;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;

    count=336;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",320,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;

    count=144;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;

    count=336;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("        ",480,count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;

    count=112;
    if (control[2].joysticknum==-1)
      {
      createmenuitem(TXT_NONE,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[2].joysticknum,0);
      }
    if (control[2].joysticknum==0)
      {
      createmenuitem(TXT_JOY1,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[2].joysticknum,1);
      }
    if (control[2].joysticknum==1)
      {
      createmenuitem(TXT_JOY2,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[2].joysticknum,2);
      }
    if (control[2].joysticknum==2)
      {
      createmenuitem(TXT_JOY3,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[2].joysticknum,3);
      }
    if (control[2].joysticknum==3)
      {
      createmenuitem(TXT_JOY4,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[2].joysticknum,-1);
      }
    count=304;
    if (control[3].joysticknum==-1)
      {
      createmenuitem(TXT_NONE,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[3].joysticknum,0);
      }
    if (control[3].joysticknum==0)
      {
      createmenuitem(TXT_JOY1,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[3].joysticknum,1);
      }
    if (control[3].joysticknum==1)
      {
      createmenuitem(TXT_JOY2,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[3].joysticknum,2);
      }
    if (control[3].joysticknum==2)
      {
      createmenuitem(TXT_JOY3,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[3].joysticknum,3);
      }
    if (control[3].joysticknum==3)
      {
      createmenuitem(TXT_JOY4,480,count,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_SET,&control[3].joysticknum,-1);
      }

    checksystemmessages();
    checkkeyboard();
    checkmouse();
    checkjoystick();
    checkmenuitems();

    if (control[2].joysticknum>=numofjoysticks)
      control[2].joysticknum=-1;
    if (control[3].joysticknum>=numofjoysticks)
      control[3].joysticknum=-1;

    setuptextdisplay();

    drawtext(TXT_PLAYER,0,80,16,1.0f,1.0f,1.0f,1.0f);
    drawtext(TXT_KEY,320,80,16,1.0f,1.0f,1.0f,1.0f);
    drawtext(TXT_JOYSTICK,480,80,16,1.0f,1.0f,1.0f,1.0f);

    drawtext(TXT_PLAYER3,0,112,16,1.0f,1.0f,1.0f,1.0f);

    count2=2;
    count=0;
    drawtext(TXT_MOVE_LEFT,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[0]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_RIGHT,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[1]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_DOWN,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[2]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_UP,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[3]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_STICK,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[4]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_JUMP,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[5]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_SLIDE,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[6]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_HEAVY,0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+1].active)
      drawtext(keyboardlabel[control[count2].key[7]],320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;
    /*
    drawtext("Start//Pause",0,144+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+17].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,144+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;
    */

    drawtext(TXT_PLAYER4,0,304,16,1.0f,1.0f,1.0f,1.0f);

    count2=3;
    count=0;
    drawtext(TXT_MOVE_LEFT,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[0]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_RIGHT,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[1]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_DOWN,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[2]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_MOVE_UP,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[3]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_AXIS,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_STICK,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[4]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_JUMP,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[5]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_SLIDE,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[6]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    drawtext(TXT_HEAVY,0,336+count*16,16,0.75f,0.75f,0.75f,1.0f);
    if (!menuitem[count+9].active)
      drawtext(keyboardlabel[control[count2].key[7]],320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    else
      drawtext("?",320,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    if (!menuitem[count+26].active)
      {
      if (control[count2].button[count]!=-1)
        drawtext(TXT_BUTTON" /i",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f,control[count2].button[count]+1);
      else
        drawtext(TXT_NONE,480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
      }
    else
      drawtext("?",480,336+count*16,16,1.0f,1.0f,1.0f,1.0f);
    count++;

    if (control[2].joysticknum!=-1)
      drawtext(joystick[control[2].joysticknum].name,480,128,10,0.5f,0.5f,0.5f,1.0f);
    if (control[3].joysticknum!=-1)
      drawtext(joystick[control[3].joysticknum].name,480,320,10,0.5f,0.5f,0.5f,1.0f);

    drawmenuitems();

    drawmousecursor(768+font.cursornum,mouse.x,mouse.y,16,1.0f,1.0f,1.0f,1.0f);

    SDL_GL_SwapWindow(sdlwindow);

    for (count=0;count<8;count++)
    if (menuitem[count+1].active)
      {
      for (count2=1;count2<323;count2++)
      if (keyboardlabel[count2][0]!=0)
      if (keyboard[count2] && !prevkeyboard[count2])
        {
        control[2].key[count]=count2;

        for (count3=0;count3<8;count3++)
        if (count3!=count)
        if (control[2].key[count3]==count2)
          control[2].key[count3]=0;

        for (count3=0;count3<8;count3++)
        if (count3!=count)
        if (control[3].key[count3]==count2)
          control[3].key[count3]=0;

        menuitem[count+1].active=0;
        }
      if (keyboard[SCAN_DELETE] && !prevkeyboard[SCAN_DELETE])
        {
        control[2].key[count]=0;
        menuitem[count+1].active=0;
        }
      }
    for (count=0;count<8;count++)
    if (menuitem[count+9].active)
      {
      for (count2=1;count2<323;count2++)
      if (keyboardlabel[count2][0]!=0)
      if (keyboard[count2] && !prevkeyboard[count2])
        {
        control[3].key[count]=count2;

        for (count3=0;count3<8;count3++)
        if (count3!=count)
        if (control[2].key[count3]==count2)
          control[2].key[count3]=0;

        for (count3=0;count3<8;count3++)
        if (count3!=count)
        if (control[3].key[count3]==count2)
          control[3].key[count3]=0;

        menuitem[count+9].active=0;
        }
      if (keyboard[SCAN_DELETE] && !prevkeyboard[SCAN_DELETE])
        {
        control[3].key[count]=0;
        menuitem[count+9].active=0;
        }
      }
    if (control[2].joysticknum!=-1)
    for (count=0;count<9;count++)
    if (menuitem[count+17].active)
      {
      for (count2=0;count2<joystick[control[2].joysticknum].numofbuttons;count2++)
      if (joystick[control[2].joysticknum].button[count2] && !prevjoystick[control[2].joysticknum].button[count2])
        {
        control[2].button[count]=count2;

        if (control[2].joysticknum!=-1)
        for (count3=0;count3<joystick[control[2].joysticknum].numofbuttons;count3++)
        if (count3!=count)
        if (control[2].button[count3]==count2)
          control[2].button[count3]=-1;

        menuitem[count+17].active=0;
        }
      if (keyboard[SCAN_DELETE] && !prevkeyboard[SCAN_DELETE])
        {
        control[2].button[count]=-1;
        menuitem[count+17].active=0;
        }
      }
    if (control[3].joysticknum!=-1)
    for (count=0;count<8;count++)
    if (menuitem[count+26].active)
      {
      for (count2=0;count2<joystick[control[3].joysticknum].numofbuttons;count2++)
      if (joystick[control[3].joysticknum].button[count2] && !prevjoystick[control[3].joysticknum].button[count2])
        {
        control[3].button[count]=count2;

        if (control[3].joysticknum!=-1)
        for (count3=0;count3<8;count3++)
        if (count3!=count)
        if (control[3].button[count3]==count2)
          control[3].button[count3]=-1;

        menuitem[count+26].active=0;
        }
      if (keyboard[SCAN_DELETE] && !prevkeyboard[SCAN_DELETE])
        {
        control[3].button[count]=-1;
        menuitem[count+26].active=0;
        }
      }
    }

  resetmenuitems();
  }

