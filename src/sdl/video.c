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

  Uint32 flags=SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL;
  if (windowinfo.fullscreen)
    flags|=SDL_WINDOW_FULLSCREEN;

  sdlwindow=SDL_CreateWindow("Gish",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    windowinfo.resolutionx,windowinfo.resolutiony,flags);

  SDL_SetWindowIcon(sdlwindow, SDL_LoadBMP("gish.bmp"));

  sdlglcontext=SDL_GL_CreateContext(sdlwindow);
  SDL_GL_MakeCurrent(sdlwindow,sdlglcontext);

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
}

int numofsdlvideomodes;
struct SDLVIDEOMODE sdlvideomode[4096];

void getvideoinfo(void)
  {
  SDL_DisplayMode sdlvideoinfo;
  Uint32 rmask,gmask,bmask,amask;
  int bpp=16;

  if (SDL_GetCurrentDisplayMode(0, &sdlvideoinfo)==0)
    {
    SDL_PixelFormatEnumToMasks(sdlvideoinfo.format,&bpp,&rmask,&gmask,&bmask,&amask);
    if (bpp==16)
      config.bitsperpixel=16;
    }
  else
    {
      config.bitsperpixel=16;
    }
  }

void listvideomodes(void)
  {
  int dispid;
  int displaycount;
  int nummodes;
  int count;
  Uint32 rmask,gmask,bmask,amask;
  int bpp;
  SDL_DisplayMode sdlmode;

  numofsdlvideomodes=0;

  displaycount=SDL_GetNumVideoDisplays();
  if (displaycount<1)
    goto fail;

  for (dispid=0;dispid<displaycount;dispid++)
    {
    nummodes=SDL_GetNumDisplayModes(dispid);
    if (nummodes<1)
      goto fail;

    for (count=0;nummodes && count<64;count++)
      {
      if (SDL_GetDisplayMode(dispid,count,&sdlmode)!=0)
        continue;
      if (SDL_PixelFormatEnumToMasks(sdlmode.format,&bpp,&rmask,&gmask,&bmask,&amask)==SDL_FALSE)
        continue;

      sdlvideomode[numofsdlvideomodes].displayid=dispid;
      sdlvideomode[numofsdlvideomodes].resolutionx=sdlmode.w;
      sdlvideomode[numofsdlvideomodes].resolutiony=sdlmode.h;
      sdlvideomode[numofsdlvideomodes].bitsperpixel=bpp;
      sdlvideomode[numofsdlvideomodes].refreshrate=sdlmode.refresh_rate;
      numofsdlvideomodes++;
      }
    }

  return;

fail:
  sdlvideomode[numofsdlvideomodes].displayid=0;
  sdlvideomode[numofsdlvideomodes].resolutionx=640;
  sdlvideomode[numofsdlvideomodes].resolutiony=480;
  sdlvideomode[numofsdlvideomodes].bitsperpixel=32;
  sdlvideomode[numofsdlvideomodes].refreshrate=60;
  numofsdlvideomodes++;
  sdlvideomode[numofsdlvideomodes].displayid=0;
  sdlvideomode[numofsdlvideomodes].resolutionx=800;
  sdlvideomode[numofsdlvideomodes].resolutiony=600;
  sdlvideomode[numofsdlvideomodes].bitsperpixel=32;
  sdlvideomode[numofsdlvideomodes].refreshrate=60;
  numofsdlvideomodes++;
  sdlvideomode[numofsdlvideomodes].displayid=0;
  sdlvideomode[numofsdlvideomodes].resolutionx=1024;
  sdlvideomode[numofsdlvideomodes].resolutiony=768;
  sdlvideomode[numofsdlvideomodes].bitsperpixel=32;
  sdlvideomode[numofsdlvideomodes].refreshrate=60;
  numofsdlvideomodes++;
  }

