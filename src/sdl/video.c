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

#include "video.h"

#include <SDL.h>
#include <GL/gl.h>
#include "event.h"
#include <game/config.h>

static const Uint8 iconmask[128]={
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x03,0xF0,0x00,
0x00,0x3F,0xFC,0x00,
0x00,0xFF,0xFF,0x00,
0x03,0xFF,0xFF,0x80,
0x07,0xFF,0xFF,0xC0,
0x0F,0xFF,0xFF,0xE0,
0x0F,0xFF,0xFF,0xF0,
0x1F,0xFF,0xFF,0xF8,
0x3F,0xFF,0xFF,0xFC,
0x3F,0xFF,0xFF,0xFC,
0x7F,0xFF,0xFF,0xFE,
0x7F,0xFF,0xFF,0xFE,
0xFF,0xFF,0xFF,0xFE,
0xFF,0xFF,0xFF,0xFE,
0xFF,0xFF,0xFF,0xFE,
0xFF,0xFF,0xFF,0xFE,
0x7F,0xFF,0xFF,0xFE,
0x3F,0xFF,0xFF,0xFE,
0x0F,0xFF,0xFF,0xFE,
0x03,0xFF,0xFF,0xFC,
0x00,0x7F,0xFF,0xF0,
0x00,0x00,0x00,0x00 };

SDL_Window *sdlwindow=NULL;
SDL_GLContext *sdlglcontext=NULL;

int numofsdldisplays;
struct SDLDISPLAY sdldisplay[32];

int numofsdlvideomodes;
struct SDLVIDEOMODE sdlvideomode[4096];

void createwindow(void)
{
  if (sdlglcontext)
    SDL_GL_DeleteContext(sdlglcontext);
  if (sdlwindow)
    SDL_DestroyWindow(sdlwindow);

  if (windowinfo.bitsperpixel==16)
    {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,5);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,0);
    }
  if (windowinfo.bitsperpixel==32)
    {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
    }
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,windowinfo.depthbits);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,windowinfo.stencilbits);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

  int windowposx=SDL_WINDOWPOS_CENTERED_DISPLAY(windowinfo.displayid);
  int windowposy=SDL_WINDOWPOS_CENTERED_DISPLAY(windowinfo.displayid);
  Uint32 flags=SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL;
  if (windowinfo.fullscreen)
    {
    windowposx=sdldisplay[windowinfo.displayid].bounds.x;
    windowposy=sdldisplay[windowinfo.displayid].bounds.y;
    flags|=(windowinfo.fullscreen==2)?
      SDL_WINDOW_FULLSCREEN_DESKTOP:
      SDL_WINDOW_FULLSCREEN;
    }

  sdlwindow=SDL_CreateWindow("Gish", windowposx, windowposy,
    windowinfo.resolutionx,windowinfo.resolutiony,flags);

  if (windowinfo.fullscreen==1)
    {
    int videomodenum=-1;
    for (int i=0;i<numofsdlvideomodes;i++)
      {
      if (windowinfo.displayid!=sdlvideomode[i].displayid)
        continue;
      if (windowinfo.resolutionx!=sdlvideomode[i].displaymode.w)
        continue;
      if (windowinfo.resolutiony!=sdlvideomode[i].displaymode.h)
        continue;
      if (windowinfo.bitsperpixel!=sdlvideomode[i].bitsperpixel)
        continue;
      if (windowinfo.refreshrate!=sdlvideomode[i].displaymode.refresh_rate)
        continue;

      videomodenum=i;
      break;
      }

    if (videomodenum>=0)
      SDL_SetWindowDisplayMode(sdlwindow,&sdlvideomode[videomodenum].displaymode);
    }
  SDL_SetWindowIcon(sdlwindow,SDL_LoadBMP("gish.bmp"));

  sdlglcontext=SDL_GL_CreateContext(sdlwindow);
  SDL_GL_MakeCurrent(sdlwindow,sdlglcontext);

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
}

void getvideoinfo(void)
  {
  SDL_DisplayMode sdlvideoinfo;
  Uint32 rmask,gmask,bmask,amask;
  int bpp=16;

  if (SDL_GetCurrentDisplayMode(0,&sdlvideoinfo)==0)
    {
    SDL_PixelFormatEnumToMasks(sdlvideoinfo.format,&config.bitsperpixel,&rmask,&gmask,&bmask,&amask);
    config.refreshrate=sdlvideoinfo.refresh_rate;
    }
  }

void listvideomodes(void)
  {
  Uint32 rmask,gmask,bmask,amask;
  numofsdlvideomodes=0;
  numofsdldisplays=SDL_GetNumVideoDisplays();

  for (int dispid=0;dispid<numofsdldisplays;dispid++)
    {
    snprintf(sdldisplay[dispid].name,32,"%d: %s",dispid, SDL_GetDisplayName(dispid));
    SDL_GetDisplayBounds(dispid,&sdldisplay[dispid].bounds);
    int nummodes=SDL_GetNumDisplayModes(dispid);
    for (int i=0;nummodes && i<64;i++)
      {
      if (SDL_GetDisplayMode(dispid,i,&sdlvideomode[numofsdlvideomodes].displaymode)!=0)
        continue;

      sdlvideomode[numofsdlvideomodes].displayid=dispid;
      sdlvideomode[numofsdlvideomodes].bitsperpixel;
      SDL_PixelFormatEnumToMasks(
        sdlvideomode[numofsdlvideomodes].displaymode.format,
        &sdlvideomode[numofsdlvideomodes].bitsperpixel,
        &rmask,&gmask,&bmask,&amask);
      numofsdlvideomodes++;
      }
    }
  }

