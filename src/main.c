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

#define CRYPTICSEA 1

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>
#include <GL/gl.h>
#include <game/glext.h>
#include <sdl/event.h>
#include <sdl/video.h>
#include <video/texture.h>
#include <video/text.h>
#include <audio/audio.h>
#include <input/keyboard.h>
#include <input/mouse.h>
#include <input/joystick.h>
#include <game/config.h>
#include <game/options.h>
#include <game/player.h>
#include <menu/menu.h>
#include <game/mainmenu.h>
#include <game/high.h>
#include <physfs.h>

int main (int argc,char *argv[])
  {
  int count;
  Uint32 flags;
  const char *temp;

  if (!PHYSFS_init(argv[0]))
    {
    fprintf(stderr,"FATAL: PHYSFS_init(): %s\n",PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return(1);
    }

  //setup fs mounts
  if (!PHYSFS_mount("gishfull.zip","",1))
    {
    fprintf(stderr,"FATAL: PHYSFS_mount(): %s\n",PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    PHYSFS_deinit();
    return(1);
    }

  loadconfig();
  loadscores();
  loadplayers();

  flags=SDL_INIT_VIDEO|SDL_INIT_TIMER;
  if (config.joystick)
    flags|=SDL_INIT_JOYSTICK;

  if (SDL_Init(flags)!=0)
    {
    fprintf(stderr,"FATAL: SDL_Init(): %s\n",SDL_GetError());
    PHYSFS_deinit();
    return(1);
    }

  getvideoinfo();

  for (count=1;count<argc;count++)
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

  SDL_ShowCursor(SDL_DISABLE);

  listvideomodes();

  createwindow();
  loadglextentions();

  for (count=0;count<2048;count++)
    glGenTextures(1,&texture[count].glname);

  if (config.joystick)
    {
    numofjoysticks=SDL_NumJoysticks();
    for (count=0;count<numofjoysticks;count++)
      {
      joy[count]=SDL_JoystickOpen(count);
      temp=SDL_JoystickName(joy[count]);
      if (temp)
        strcpy(joystick[count].name,temp);
      else
        strcpy(joystick[count].name,"Unknown");
      joystick[count].numofbuttons=SDL_JoystickNumButtons(joy[count]);
      joystick[count].numofhats=SDL_JoystickNumHats(joy[count]);
      }

    SDL_JoystickEventState(SDL_IGNORE);
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

  SDL_GL_DeleteContext(sdlglcontext);
  SDL_DestroyWindow(sdlwindow);
  SDL_Quit();
  PHYSFS_deinit();

  return(0);
  }
