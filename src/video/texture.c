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

#include "texture.h"

#include <stdlib.h>
#include <stdio.h>
#include <game/utils.h>
#include <SDL.h>
#include <physfs.h>
#include <string.h>
#include <SDL_opengl.h>
#include <sdl/platform.h>
#include <sdl/file.h>
#include <unistd.h>

struct TGAHEADER
  {
  PHYSFS_uint8  identityfieldlen;
  PHYSFS_uint8  colourmaptype;
  PHYSFS_uint8  imagetype;
  PHYSFS_uint16 colourmaporg;
  PHYSFS_uint16 colourmaplen;
  PHYSFS_uint8  colourmapentrysize;
  PHYSFS_uint16 originx;
  PHYSFS_uint16 originy;
  PHYSFS_uint16 imagewidth;
  PHYSFS_uint16 imageheight;
  PHYSFS_uint8  bitsperpixel;
  PHYSFS_uint8  imagedescriptor;
  };

char lasttextureloaded[32];
struct TEXTURE texture[2048];

int readtgaheader(PHYSFS_file *fp,struct TGAHEADER *out)
  {
  struct TGAHEADER tga;
  size_t filepos;

  //read tga header
  PHYSFS_readBytes(fp,&tga.identityfieldlen,1);
  PHYSFS_readBytes(fp,&tga.colourmaptype,1);
  PHYSFS_readBytes(fp,&tga.imagetype,1);
  PHYSFS_readULE16(fp,&tga.colourmaporg);
  PHYSFS_readULE16(fp,&tga.colourmaplen);
  PHYSFS_readBytes(fp,&tga.colourmapentrysize,1);
  PHYSFS_readULE16(fp,&tga.originx);
  PHYSFS_readULE16(fp,&tga.originy);
  PHYSFS_readULE16(fp,&tga.imagewidth);
  PHYSFS_readULE16(fp,&tga.imageheight);
  PHYSFS_readBytes(fp,&tga.bitsperpixel,1);
  PHYSFS_readBytes(fp,&tga.imagedescriptor,1);

  //sanity checks :eyes:
  if (tga.colourmaptype)
    {
#ifdef DEBUG
    fprintf(stderr,"readtgaheader(): indexed tgas are unsupported\n");
#endif
    return 1;
    }
  if (tga.bitsperpixel!=32&&tga.bitsperpixel!=24)
    {
#ifdef DEBUG
    fprintf(stderr,"readtgaheader(): unsupported bitdepth %d\n",tga.bitsperpixel);
#endif
    return 1;
    }
  /*
#ifdef DEBUG
  if ((tgabitsperpixel==32&&(tgaimagedescriptor&0x7)!=8)||(tgabitsperpixel==24&&(tgaimagedescriptor&0x7)!=0))
    {
    fprintf(stderr,"WARN: loadtexturetga(): attribute bits look wrong (%dbpp, %d), corrupted maybe?\n",tgabitsperpixel,tgaimagedescriptor&0x7);
    }
  printf("origin %s\n",(tgaimagedescriptor&(1<<5))?"top-left":"bottom-left");
  static const char* storagetype[4]=
    {
    "non-interleaved",
    "two-way (even/odd) interleaving",
    "four way interleaving",
    "reserved",
    };
  printf("%s\n",storagetype[(tgaimagedescriptor>>6)&0x3]);
#endif
  */

  //skip identification field nonsense
  if (tga.identityfieldlen>0)
    {
    filepos=(size_t)PHYSFS_tell(fp);
    PHYSFS_seek(fp,filepos+tga.identityfieldlen);
    }

  memcpy(out,&tga,sizeof(struct TGAHEADER));
  return 0;
  }

SDL_Surface *createsurfacefromtga(const struct TGAHEADER *tga,PHYSFS_file *fp)
  {
  //create a surface to load to
  Uint32 pixelformatenum=(tga->bitsperpixel==32)?SDL_PIXELFORMAT_RGBA32:SDL_PIXELFORMAT_RGB24;
  SDL_Surface *img=SDL_CreateRGBSurfaceWithFormat(0,tga->imagewidth,tga->imageheight,tga->bitsperpixel,pixelformatenum);
  if (!img)
    {
#ifdef DEBUG
    fprintf(stderr,"SDL_CreateRGBSurfaceWithFormat(): %s\n",SDL_GetError());
#endif
    return NULL;
    }

  //copy pixels
  if (tga->bitsperpixel==32)
    {
    for (int i=0;i<tga->imageheight;i++)
      {
      int y=(tga->imagedescriptor&(1<<5))?i:tga->imageheight-i-1;
      Uint32 *ptr=(Uint32 *)((Uint8 *)img->pixels+y*img->pitch);
      for (int j=0;j<tga->imagewidth;j++)
        {
        SDL_Colour c;
        PHYSFS_readBytes(fp,&c.b,1);
        PHYSFS_readBytes(fp,&c.g,1);
        PHYSFS_readBytes(fp,&c.r,1);
        PHYSFS_readBytes(fp,&c.a,1);
        (*ptr++)=SDL_MapRGBA(img->format,c.r,c.g,c.b,c.a);
        }
      }
    }
  else if (tga->bitsperpixel==24)
    {
    //TODO: needs testing
    for (int i=0;i<tga->imageheight;i++)
      {
      int y=(tga->imagedescriptor&(1<<5))?i:tga->imageheight-i-1;
      Uint8 *ptr=(Uint8 *)img->pixels+y*img->pitch;
      for (int j=0;j<tga->imagewidth;j++)
        {
        SDL_Colour c;
        PHYSFS_readBytes(fp,&c.b,1);
        PHYSFS_readBytes(fp,&c.g,1);
        PHYSFS_readBytes(fp,&c.r,1);
        Uint32 pix=SDL_MapRGB(img->format,c.r,c.g,c.b);
        memcpy(ptr,&pix,3);
        ptr+=3;
        }
      }
    }

  return img;
  }

void loadtexturetga(int texturenum,char *filename,int mipmap,int wraps,int wrapt,int magfilter,int minfilter)
  {
  struct TGAHEADER tga;
  PHYSFS_file *fp;

  //open file
  fp=PHYSFS_openRead(filename);
  if (!fp)
    {
#ifdef DEBUG
    fprintf(stderr,"PHYSFS_openRead(): %s\n",PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    fprintf(stderr,"Texture Load Failed: %d \"%s\"\n",texturenum,filename);
#endif
    return;
    }

  //read tga header
  if (readtgaheader(fp,&tga))
    {
#ifdef DEBUG
    fprintf(stderr,"Texture Load Failed: %d \"%s\"\n",texturenum,filename);
#endif
    PHYSFS_close(fp);
    return;
    }

  //load tga into surface
  SDL_Surface *img=createsurfacefromtga(&tga,fp);
  if (!img)
    {
#ifdef DEBUG
    fprintf(stderr,"Texture Load Failed: %d \"%s\"\n",texturenum,filename);
#endif
    PHYSFS_close(fp);
    return;
    }

  PHYSFS_close(fp);

  //setup texture attributes
  texture[texturenum].sizex=(int)tga.imagewidth;
  texture[texturenum].sizey=(int)tga.imageheight;
  texture[texturenum].mipmaplevels=1;
  texture[texturenum].format=(tga.bitsperpixel==32)?GL_RGBA:GL_RGB;
  texture[texturenum].alphamap=0;
  texture[texturenum].normalmap=0;
  texture[texturenum].glossmap=0;
  texture[texturenum].wraps=wraps;
  texture[texturenum].wrapt=wrapt;
  texture[texturenum].magfilter=magfilter;
  texture[texturenum].minfilter=minfilter;
  texture[texturenum].isalpha=(tga.bitsperpixel==32)?1:0;

  //copy texture data
  const int bytesperpixel=tga.bitsperpixel/8;
  if (texture[texturenum].rgba[0]!=NULL)
    free(texture[texturenum].rgba[0]);
  texture[texturenum].rgba[0]=(unsigned int *)malloc((size_t)texture[texturenum].sizex*texture[texturenum].sizey*bytesperpixel);
  for (int i=0;i<texture[texturenum].sizey;i++)
    {
    Uint8 *src=(Uint8 *)img->pixels+i*img->pitch;
    Uint8 *dst=(Uint8 *)texture[texturenum].rgba[0]+(i*texture[texturenum].sizex*bytesperpixel);
    memcpy(dst,src,(size_t)texture[texturenum].sizex*bytesperpixel);
    }

  SDL_FreeSurface(img);

  //create opengl texture
  if (mipmap)
    generatemipmap(texturenum);
  setuptexture(texturenum);
  }

void loadtexturetgaparts(int texturenum,char *filename,const SDL_Rect *rect,int numrect)
  {
  struct TGAHEADER tga;
  PHYSFS_file *fp;

  //open file
  fp=PHYSFS_openRead(filename);
  if (!fp)
    {
#ifdef DEBUG
    fprintf(stderr,"PHYSFS_openRead(): %s\n",PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    fprintf(stderr,"Texture Load Failed: %d \"%s\"\n",texturenum,filename);
#endif
    return;
    }

  //read tga header
  if (readtgaheader(fp,&tga))
    {
#ifdef DEBUG
    fprintf(stderr,"Texture Load Failed: %d \"%s\"\n",texturenum,filename);
#endif
    PHYSFS_close(fp);
    return;
    }

  //load tga into surface
  SDL_Surface *img=createsurfacefromtga(&tga,fp);
  if (!img)
    {
#ifdef DEBUG
    fprintf(stderr,"Texture Load Failed: %d \"%s\"\n",texturenum,filename);
#endif
    PHYSFS_close(fp);
    return;
    }

  PHYSFS_close(fp);

  int glformat=(tga.bitsperpixel==32)?GL_RGBA:GL_RGB;
  int hasalpha=(tga.bitsperpixel==32)?1:0;
  for (int j=0;j<numrect;j++)
    {
    //setup texture attributes
    texture[texturenum].sizex=rect[j].w;
    texture[texturenum].sizey=rect[j].h;
    texture[texturenum].mipmaplevels=1;
    texture[texturenum].format=glformat;
    texture[texturenum].alphamap=0;
    texture[texturenum].normalmap=0;
    texture[texturenum].glossmap=0;
    texture[texturenum].wraps=GL_CLAMP;
    texture[texturenum].wrapt=GL_CLAMP;
    texture[texturenum].magfilter=GL_NEAREST;
    texture[texturenum].minfilter=GL_NEAREST;
    texture[texturenum].isalpha=hasalpha;

    //copy texture data
    const int bytesperpixel=tga.bitsperpixel/8;
    const int bytesperrow=(MIN(rect[j].x+texture[texturenum].sizex,tga.imagewidth)-rect[j].x)*bytesperpixel;
    const int numcolums=MIN(rect[j].y+texture[texturenum].sizey,tga.imageheight)-rect[j].y;

    if (texture[texturenum].rgba[0]!=NULL)
      free(texture[texturenum].rgba[0]);

    const size_t memsize=(size_t)texture[texturenum].sizex*texture[texturenum].sizey*bytesperpixel;
    texture[texturenum].rgba[0]=(unsigned int *)malloc(memsize);
    memset(texture[texturenum].rgba[0],0xFF,memsize);
    for (int i=0;i<numcolums;i++)
      {
      Uint8 *src=(Uint8 *)img->pixels+rect[j].x*bytesperpixel+(i+rect[j].y)*img->pitch;
      Uint8 *dst=(Uint8 *)texture[texturenum].rgba[0]+(i*texture[texturenum].sizex*bytesperpixel);
      memcpy(dst,src,(size_t)bytesperrow);
      }

    //create opengl texture
    setuptexture(texturenum);
    texturenum++;
    }

  SDL_FreeSurface(img);
  }

void generatemipmap(int texturenum)
  {
  int count,count2,count3,count4;
  int mipmaplevel;
  int mipmaplevelmax;
  unsigned int red,green,blue,alpha,temp;

  if (texture[texturenum].format!=GL_RGBA)
    return;

  mipmaplevelmax=1;
  while ((texture[texturenum].sizex>>mipmaplevelmax)!=0)
    mipmaplevelmax++;

  texture[texturenum].mipmaplevels=mipmaplevelmax;

  for (mipmaplevel=1;mipmaplevel<mipmaplevelmax;mipmaplevel++)
    {
    free(texture[texturenum].rgba[mipmaplevel]);
    texture[texturenum].rgba[mipmaplevel]=(unsigned int *) malloc((texture[texturenum].sizex>>mipmaplevel)*(texture[texturenum].sizey>>mipmaplevel)*4);
    if (texture[texturenum].normalmap)
      {
      free(texture[texturenum].normal[mipmaplevel]);
      texture[texturenum].normal[mipmaplevel]=(unsigned int *) malloc((texture[texturenum].sizex>>mipmaplevel)*(texture[texturenum].sizey>>mipmaplevel)*4);
      }
    for (count=0;count<(texture[texturenum].sizey>>mipmaplevel);count++)
      for (count2=0;count2<(texture[texturenum].sizex>>mipmaplevel);count2++)
        {
        red=0;
        green=0;
        blue=0;
        alpha=0;
        for (count3=0;count3<2;count3++)
        for (count4=0;count4<2;count4++)
          {
          temp=texture[texturenum].rgba[mipmaplevel-1][(count*2+count3)*(texture[texturenum].sizex>>(mipmaplevel-1))+(count2*2+count4)];
          red+=temp&0xFF;
          green+=(temp>>8)&0xFF;
          blue+=(temp>>16)&0xFF;
          alpha+=(temp>>24)&0xFF;
          }

        red>>=2;
        green>>=2;
        blue>>=2;
        alpha>>=2;
        texture[texturenum].rgba[mipmaplevel][count*(texture[texturenum].sizex>>mipmaplevel)+count2]=(alpha<<24)+(blue<<16)+(green<<8)+red;

        if (texture[texturenum].normalmap)
          {
          red=0;
          green=0;
          blue=0;
          alpha=0;
          for (count3=0;count3<2;count3++)
          for (count4=0;count4<2;count4++)
            {
            temp=texture[texturenum].normal[mipmaplevel-1][(count*2+count3)*(texture[texturenum].sizex>>(mipmaplevel-1))+(count2*2+count4)];
            red+=temp&0xFF;
            green+=(temp>>8)&0xFF;
            blue+=(temp>>16)&0xFF;
            alpha+=(temp>>24)&0xFF;
            }
  
          red>>=2;
          green>>=2;
          blue>>=2;
          alpha>>=2;
          texture[texturenum].normal[mipmaplevel][count*(texture[texturenum].sizex>>mipmaplevel)+count2]=(alpha<<24)+(blue<<16)+(green<<8)+red;
          }
        }
    }
  }

void setuptexture(int texturenum)
  {
  int count,count2;

  glBindTexture(GL_TEXTURE_2D,texture[texturenum].glname);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,texture[texturenum].wraps);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,texture[texturenum].wrapt);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,texture[texturenum].magfilter);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,texture[texturenum].minfilter);

  texture[texturenum].isalpha=0;

  for (count=0;count<texture[texturenum].sizey;count++)
  for (count2=0;count2<texture[texturenum].sizex;count2++)
    {
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    if ((texture[texturenum].rgba[0][count*texture[texturenum].sizex+count2]>>24)!=255)
      {
      texture[texturenum].isalpha=1;
      texture[texturenum].alphamap=1;
      }
#else
    if ((texture[texturenum].rgba[0][count*texture[texturenum].sizex+count2]&255)!=255)
      {
      texture[texturenum].isalpha=1;
      texture[texturenum].alphamap=1;
      }
#endif
    }

  for (count=0;count<texture[texturenum].mipmaplevels;count++)
    glTexImage2D(GL_TEXTURE_2D,count,texture[texturenum].format,(texture[texturenum].sizex>>count),(texture[texturenum].sizey>>count),
                 0,texture[texturenum].format,GL_UNSIGNED_BYTE,texture[texturenum].rgba[count]);

  if (texture[texturenum].normalmap)
    {
    glBindTexture(GL_TEXTURE_2D,texture[texturenum].glnamenormal);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,texture[texturenum].wraps);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,texture[texturenum].wrapt);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,texture[texturenum].magfilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,texture[texturenum].minfilter);

    for (count=0;count<texture[texturenum].mipmaplevels;count++)
      glTexImage2D(GL_TEXTURE_2D,count,GL_RGBA,(texture[texturenum].sizex>>count),(texture[texturenum].sizey>>count),
                   0,GL_RGBA,GL_UNSIGNED_BYTE,texture[texturenum].normal[count]);
    }
  if (texture[texturenum].glossmap)
    {
    glBindTexture(GL_TEXTURE_2D,texture[texturenum].glnamegloss);
  
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,texture[texturenum].wraps);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,texture[texturenum].wrapt);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,texture[texturenum].magfilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,texture[texturenum].minfilter);
  
    for (count=0;count<texture[texturenum].mipmaplevels;count++)
      glTexImage2D(GL_TEXTURE_2D,count,GL_ALPHA,(texture[texturenum].sizex>>count),(texture[texturenum].sizey>>count),
                   0,GL_ALPHA,GL_UNSIGNED_BYTE,texture[texturenum].gloss[count]);
    }
  }
