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

#include "socket.h"

#include "sdl/platform.h"
//#include <SDL2/SDL_video.h>
#include "sdl/video.h"
#ifdef WINDOZE
  #include <io.h>
  #include <windows.h>
  #include <winsock.h>
#elif defined(DETLEF)
  #include <stdlib.h>
  #include <stdio.h>
#endif

unsigned int rotint(unsigned int x,int rotnum)
  {
  int count;
  unsigned int y;

  y=0;
  for (count=0;count<32;count++)
    {
    y<<=1;
    y|=((x>>((count+rotnum)&31))&1);
    }

  return(y);
  }

void launchwebpage(char *webpagename)
  {
#ifdef WINDOZE
  SDL_MinimizeWindow(sdlwindow);
  ShellExecute(NULL,"open",webpagename,"","c:\\",SW_SHOWNORMAL);
#elif defined(THINKSTUPID)
  /*
  OSStatus err;
  ICInstance inst;
  long length,start,end;

  SDL_MinimizeWindow(sdlwindow);

  err=ICStart(&inst,'Gish');
  if (err==noErr)
    {
    length=strlen(webpagename);
    start=0;
    end=length;
    ICLaunchURL(inst,NULL,webpagename,length,&start,&end);
    }
  ICStop(inst);
  */
#elif defined(DETLEF)
  static const char *handlers[]=
    {
    "xdg-open",
    "kde-open5",
    "exo-open",
    "x-www-browser",
    "firefox",
    "chromium",
    "firefox-esr",
    NULL
    };

  SDL_MinimizeWindow(sdlwindow);

  char pathbuf[32];
  const char **hnd=handlers;
  while (*hnd)
    {
    snprintf(pathbuf,32,"/usr/bin/%s",*hnd);
    if (!gexec(pathbuf,webpagename))
      return;
    snprintf(pathbuf,32,"/usr/local/bin/%s",*hnd);
    if (!gexec(pathbuf,webpagename))
      return;
    snprintf(pathbuf,32,"/bin/%s",*hnd);
    if (!gexec(pathbuf,webpagename))
      return;
    ++hnd;
    }
//#ifdef DEBUG
  fprintf(stderr,"No usable launcher or browser found");
//#endif
#endif
  }
