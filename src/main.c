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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_joystick.h>
#include "game/glext.h"
#include "sdl/event.h"
#include "sdl/video.h"
#include "sdl/string.h"
#include "video/texture.h"
#include "video/text.h"
#include "audio/audio.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/joystick.h"
#include "game/config.h"
#include "game/options.h"
#include "game/player.h"
#include "menu/menu.h"
#include "game/mainmenu.h"
#include "game/high.h"

int main (int argc,char *argv[])
  {
  loadconfig();
  loadscores();
  loadplayers();

  Uint32 flags=SDL_INIT_VIDEO;
  if (config.joystick)
    flags|=SDL_INIT_JOYSTICK;

  if (!SDL_Init(flags))
    {
    return(1);
    }

  getvideoinfo();

  for (int count=1;count<argc;count++)
    {
    if (strcmp("-nosound",argv[count])==0)
      {
      config.sound=0;
      option.sound=0;
      option.music=0;
      }
    if (strcmp("-sound",argv[count])==0)
      config.sound=1;
    if (strcmp("-nomusic",argv[count])==0)
      option.music=0;
    }

  saveconfig();

  SDL_HideCursor();

  listvideomodes();

  createwindow();
  loadglextentions();

  for (int count=0;count<2048;count++)
    glGenTextures(1,&texture[count].glname);

  if (config.joystick)
    {
    SDL_JoystickID *joyids=SDL_GetJoysticks(&numofjoysticks);
    if (numofjoysticks > 16)
      numofjoysticks = 16;
    for (int count=0;count<numofjoysticks;count++)
      {
      joy[count]=SDL_OpenJoystick(joyids[count]);
      const char *temp=SDL_GetJoystickName(joy[count]);
      if (temp)
        {
        gstrlcpy(joystick[count].name,temp,64);
        }
      else
        {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"SDL_GetJoystickName: %s",SDL_GetError());
        gstrlcpy(joystick[count].name,"Unknown",64);
        }
      joystick[count].numofbuttons=SDL_GetNumJoystickButtons(joy[count]);
      joystick[count].numofhats=SDL_GetNumJoystickHats(joy[count]);
      }

    SDL_SetJoystickEventsEnabled(false);
    SDL_free(joyids);
    }

  font.texturenum=0;
  font.cursornum=0;
  font.sizex=640;
  font.sizey=480;

  loadtexturetga(1000,"font00.tga",0,GL_CLAMP,GL_CLAMP,GL_LINEAR,GL_LINEAR);
  loadtexturetga(1001,"font01.tga",0,GL_CLAMP,GL_CLAMP,GL_LINEAR,GL_LINEAR);

  loadtexturetga(768,"mouse00.tga",0,GL_CLAMP,GL_CLAMP,GL_LINEAR,GL_LINEAR);
  loadtexturetga(769,"mouse00.tga",0,GL_CLAMP,GL_CLAMP,GL_LINEAR,GL_LINEAR);

  setupmenuitems();

  if (!glext.multitexture)
    {
    notsupportedmenu();
    SDL_Quit();
    return(0);
    }
    
  if (config.sound)
    setupaudio();

  mainmenu();

  saveconfig();
  savescores();
  saveplayers();

  if (config.sound)
    shutdownaudio();

  SDL_GL_DestroyContext(sdlglcontext);
  SDL_DestroyWindow(sdlwindow);
  SDL_Quit();

  return(0);
  }
