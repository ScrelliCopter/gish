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
#include <SDL.h>
#include <physfs.h>
#include <string.h>
#include <GL/gl.h>
#include <sdl/platform.h>
#include <sdl/file.h>

struct TGAHEADER
  {
  unsigned char imagetypecode;
  short int imagewidth;
  short int imageheight;
  unsigned char pixeldepth;
  };

char lasttextureloaded[32];
struct TEXTURE texture[2048];

void loadtexturetgafix(int texturenum,char *filename,int mipmap,int wraps,int wrapt,int magfilter,int minfilter)
  {
  PHYSFS_file *fp;
  size_t filepos;

  PHYSFS_uint8  tgaidentityfieldlen;
  PHYSFS_uint8  tgacolourmaptype;
  PHYSFS_uint8  tgaimagetype;
  PHYSFS_uint16 tgacolourmaporg;
  PHYSFS_uint16 tgacolourmaplen;
  PHYSFS_uint8  tgacolourmapentrysize;
  PHYSFS_uint16 tgaoriginx;
  PHYSFS_uint16 tgaoriginy;
  PHYSFS_uint16 tgaimagewidth;
  PHYSFS_uint16 tgaimageheight;
  PHYSFS_uint8  tgabitsperpixel;
  PHYSFS_uint8  tgaimagedescriptor;

  //open file
  fp=PHYSFS_openRead(filename);
  if (!fp)
    {
#ifdef DEBUG
    fprintf(stderr,"Texture Load Failed: %d \"%s\"\n",texturenum,filename);
    fprintf(stderr,"PHYSFS_openRead(): %s\n",PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
#endif
    return;
    }

  //read tga header
  PHYSFS_readBytes(fp,&tgaidentityfieldlen,1);
  PHYSFS_readBytes(fp,&tgacolourmaptype,1);
  PHYSFS_readBytes(fp,&tgaimagetype,1);
  PHYSFS_readULE16(fp,&tgacolourmaporg);
  PHYSFS_readULE16(fp,&tgacolourmaplen);
  PHYSFS_readBytes(fp,&tgacolourmapentrysize,1);
  PHYSFS_readULE16(fp,&tgaoriginx);
  PHYSFS_readULE16(fp,&tgaoriginy);
  PHYSFS_readULE16(fp,&tgaimagewidth);
  PHYSFS_readULE16(fp,&tgaimageheight);
  PHYSFS_readBytes(fp,&tgabitsperpixel,1);
  PHYSFS_readBytes(fp,&tgaimagedescriptor,1);

  //sanity checks :eyes:
  if (tgacolourmaptype)
    {
#ifdef DEBUG
    fprintf(stderr,"Texture Load Failed: %d \"%s\"\n",texturenum,filename);
    fprintf(stderr,"loadtexturetga(): indexed tgas are unsupported\n");
#endif
    PHYSFS_close(fp);
    return;
    }
  if (tgabitsperpixel!=32&&tgabitsperpixel!=24)
    {
#ifdef DEBUG
    fprintf(stderr,"Texture Load Failed: %d \"%s\"\n",texturenum,filename);
    fprintf(stderr,"loadtexturetga(): unsupported bitdepth %d\n",tgabitsperpixel);
#endif
    PHYSFS_close(fp);
    return;
    }
#ifdef DEBUG
  if ((tgabitsperpixel==32&&(tgaimagedescriptor&0x7)!=8)||(tgabitsperpixel==24&&(tgaimagedescriptor&0x7)!=0))
    {
    fprintf(stderr,"WARN: loadtexturetga(): attribute bits look wrong (%dbpp, %d), corrupted maybe?\n",tgabitsperpixel,tgaimagedescriptor&0x7);
    }
  //printf("origin %s\n",(tgaimagedescriptor&(1<<5))?"top-left":"bottom-left");
  static const char* storagetype[4]=
    {
    "non-interleaved",
    "two-way (even/odd) interleaving",
    "four way interleaving",
    "reserved",
    };
  printf("%s\n",storagetype[(tgaimagedescriptor>>6)&0x3]);
#endif

  //skip identification field nonsense
  if (tgaidentityfieldlen>0)
    {
    filepos=(size_t)PHYSFS_tell(fp);
    PHYSFS_seek(fp,filepos+tgaidentityfieldlen);
    }

  //create a surface to load to
  Uint32 pixelformat=(tgabitsperpixel==32)?SDL_PIXELFORMAT_RGBA32:SDL_PIXELFORMAT_RGB24;
  SDL_Surface *img=SDL_CreateRGBSurfaceWithFormat(0,tgaimagewidth,tgaimageheight,tgabitsperpixel,pixelformat);
  if (!img)
    {
#ifdef DEBUG
    fprintf(stderr,"Texture Load Failed: %d \"%s\"\n",texturenum,filename);
    fprintf(stderr,"SDL_CreateRGBSurfaceWithFormat(): %s\n",SDL_GetError());
#endif
    PHYSFS_close(fp);
    return;
    }

  //copy pixels
  const int bytesperpixel=tgabitsperpixel/8;
  if (tgabitsperpixel==32)
    {
    texture[texturenum].isalpha=1;
    for (int i=0;i<tgaimageheight;i++)
      {
      int y=(tgaimagedescriptor&(1<<5))?i:tgaimageheight-i-1;
      Uint32 *ptr=(Uint32 *)((Uint8 *)img->pixels+y*img->pitch);
      for (int j=0;j<tgaimagewidth;j++)
        {
        PHYSFS_readULE32(fp,(ptr));
        ptr++;
        }
      }
    }
  else if (tgabitsperpixel==24)
    {
    texture[texturenum].isalpha=0;
    for (int i=0;i<tgaimageheight;i++)
      {
      int y=(tgaimagedescriptor&(1<<5))?i:tgaimageheight-i-1;
      Uint8 *ptr=(Uint8 *)img->pixels+y*img->pitch;
      for (int j=0;j<tgaimagewidth;j++)
        {
        //FIXME: probably make this bi-endian
        PHYSFS_readBytes(fp,ptr,3);
        ptr+=3;
        }
      }
    }

  //free file handle
  PHYSFS_close(fp);

  //setup texture attributes
  texture[texturenum].sizex=(int)tgaimagewidth;
  texture[texturenum].sizey=(int)tgaimageheight;
  texture[texturenum].mipmaplevels=1;
  texture[texturenum].format=(tgabitsperpixel==32)?GL_RGBA:GL_RGB;
  texture[texturenum].alphamap=0;
  texture[texturenum].normalmap=0;
  texture[texturenum].glossmap=0;
  texture[texturenum].wraps=wraps;
  texture[texturenum].wrapt=wrapt;
  texture[texturenum].magfilter=magfilter;
  texture[texturenum].minfilter=minfilter;

  //copy texture data
  if (texture[texturenum].rgba[0]!=NULL)
    free(texture[texturenum].rgba[0]);
  texture[texturenum].rgba[0]=(unsigned int *)malloc((size_t)texture[texturenum].sizex*texture[texturenum].sizey*bytesperpixel);
  for (int i=0;i<tgaimageheight;i++)
    {
    Uint8 *ptr=(Uint8 *)img->pixels+i*img->pitch;
    memcpy((Uint8 *)texture[texturenum].rgba[0]+i*texture[texturenum].sizex,ptr,(size_t)texture[texturenum].sizex*bytesperpixel);
    }

  //create opengl texture
  if (mipmap)
    generatemipmap(texturenum);
  setuptexture(texturenum);
  }

void loadtexturetga(int texturenum,char *filename,int mipmap,int wraps,int wrapt,int magfilter,int minfilter)
  {
  int count,count2;
  unsigned int red,green,blue,alpha;
  int changeddir;
  unsigned char origin;
  struct TGAHEADER tgaheader;
  unsigned int* imagedata;
  FILE *fp;

  changeddir=chdir("texture");

  if ((fp=fopen(filename,"rb"))==NULL)
    {
#ifdef DEBUG
    printf("Texture Load Failed: %d\n",texturenum);
#endif

    if (changeddir==0)
      chdir("..");
    return;
    }

  fseek(fp,2,SEEK_CUR);
  fread2(&tgaheader.imagetypecode,1,1,fp);
  if (tgaheader.imagetypecode!=2 && tgaheader.imagetypecode!=3)
    {
#ifdef DEBUG
    printf("Texture Bad Format: %d\n",texturenum);
#endif

    fclose(fp);

    if (changeddir==0)
      chdir("..");
    return;
    }

  fseek(fp,9,SEEK_CUR);
  fread2(&tgaheader.imagewidth,2,1,fp);
  fread2(&tgaheader.imageheight,2,1,fp);
  fread2(&tgaheader.pixeldepth,1,1,fp);
  fread2(&origin,1,1,fp);
  origin=(origin>>4)&3;

  texture[texturenum].isalpha=0;

  imagedata=malloc(tgaheader.imagewidth*tgaheader.imageheight*sizeof(uint32_t));

  for (count=0;count<tgaheader.imageheight;count++)
  for (count2=0;count2<tgaheader.imagewidth;count2++)
    {
    blue=(unsigned int)fgetc(fp);
    green=(unsigned int)fgetc(fp);
    red=(unsigned int)fgetc(fp);
    if (tgaheader.pixeldepth==32)
      alpha=(unsigned int)fgetc(fp);
    else
      alpha=255;

    if (alpha!=255)
      texture[texturenum].isalpha=1;

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    if (origin==0)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+count2]=(alpha<<24)+(blue<<16)+(green<<8)+red;
    if (origin==1)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(alpha<<24)+(blue<<16)+(green<<8)+red;
    if (origin==2)
      imagedata[count*tgaheader.imagewidth+count2]=(alpha<<24)+(blue<<16)+(green<<8)+red;
    if (origin==3)
      imagedata[count*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(alpha<<24)+(blue<<16)+(green<<8)+red;
#else
    if (origin==0)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+count2]=(red<<24)+(green<<16)+(blue<<8)+alpha;
    if (origin==1)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(red<<24)+(green<<16)+(blue<<8)+alpha;
    if (origin==2)
      imagedata[count*tgaheader.imagewidth+count2]=(red<<24)+(green<<16)+(blue<<8)+alpha;
    if (origin==3)
      imagedata[count*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(red<<24)+(green<<16)+(blue<<8)+alpha;
#endif
    }

  fclose(fp);

  if (changeddir==0)
    chdir("..");

  if ((tgaheader.imagewidth&(tgaheader.imagewidth-1))!=0)
    return;
  if ((tgaheader.imageheight&(tgaheader.imageheight-1))!=0)
    return;

  texture[texturenum].sizex=tgaheader.imagewidth;
  texture[texturenum].sizey=tgaheader.imageheight;
  texture[texturenum].mipmaplevels=1;
  texture[texturenum].format=GL_RGBA;
  texture[texturenum].alphamap=0;
  texture[texturenum].normalmap=0;
  texture[texturenum].glossmap=0;
  texture[texturenum].wraps=wraps;
  texture[texturenum].wrapt=wrapt;
  texture[texturenum].magfilter=magfilter;
  texture[texturenum].minfilter=minfilter;

  free(texture[texturenum].rgba[0]);
  texture[texturenum].rgba[0]=(unsigned int *) malloc(texture[texturenum].sizex*texture[texturenum].sizey*4);

  for (count=0;count<texture[texturenum].sizey;count++)
  for (count2=0;count2<texture[texturenum].sizex;count2++)
    {
    texture[texturenum].rgba[0][count*texture[texturenum].sizex+count2]=imagedata[count*tgaheader.imagewidth+count2];

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    if ((texture[texturenum].rgba[0][count*texture[texturenum].sizex+count2]>>24)!=255)
      texture[texturenum].alphamap=1;
#else
    if ((texture[texturenum].rgba[0][count*texture[texturenum].sizex+count2]&255)!=255)
      texture[texturenum].alphamap=1;
#endif
    }

  free(imagedata);

  if (mipmap)
    generatemipmap(texturenum);
  setuptexture(texturenum);
  }

void loadtexturetganodir(int texturenum,char *filename,int mipmap,int wraps,int wrapt,int magfilter,int minfilter)
  {
  int count,count2;
  unsigned int red,green,blue,alpha;
  unsigned char origin;
  struct TGAHEADER tgaheader;
  unsigned int* imagedata;
  FILE *fp;

  if ((fp=fopen(filename,"rb"))==NULL)
    {
#ifdef DEBUG
    printf("Texture Load Failed: %d\n",texturenum);
#endif

    return;
    }

  fseek(fp,2,SEEK_CUR);
  fread2(&tgaheader.imagetypecode,1,1,fp);
  if (tgaheader.imagetypecode!=2 && tgaheader.imagetypecode!=3)
    {
#ifdef DEBUG
    printf("Texture Bad Format: %d\n",texturenum);
#endif

    fclose(fp);

    return;
    }

  fseek(fp,9,SEEK_CUR);
  fread2(&tgaheader.imagewidth,2,1,fp);
  fread2(&tgaheader.imageheight,2,1,fp);
  fread2(&tgaheader.pixeldepth,1,1,fp);
  fread2(&origin,1,1,fp);
  origin=(origin>>4)&3;

  texture[texturenum].isalpha=0;

  imagedata=malloc(tgaheader.imagewidth*tgaheader.imageheight*sizeof(uint32_t));

  for (count=0;count<tgaheader.imageheight;count++)
  for (count2=0;count2<tgaheader.imagewidth;count2++)
    {
    blue=(unsigned int)fgetc(fp);
    green=(unsigned int)fgetc(fp);
    red=(unsigned int)fgetc(fp);
    if (tgaheader.pixeldepth==32)
      alpha=(unsigned int)fgetc(fp);
    else
      alpha=255;

    if (alpha!=255)
      texture[texturenum].isalpha=1;

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    if (origin==0)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+count2]=(alpha<<24)+(blue<<16)+(green<<8)+red;
    if (origin==1)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(alpha<<24)+(blue<<16)+(green<<8)+red;
    if (origin==2)
      imagedata[count*tgaheader.imagewidth+count2]=(alpha<<24)+(blue<<16)+(green<<8)+red;
    if (origin==3)
      imagedata[count*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(alpha<<24)+(blue<<16)+(green<<8)+red;
#else
    if (origin==0)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+count2]=(red<<24)+(green<<16)+(blue<<8)+alpha;
    if (origin==1)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(red<<24)+(green<<16)+(blue<<8)+alpha;
    if (origin==2)
      imagedata[count*tgaheader.imagewidth+count2]=(red<<24)+(green<<16)+(blue<<8)+alpha;
    if (origin==3)
      imagedata[count*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(red<<24)+(green<<16)+(blue<<8)+alpha;
#endif
    }

  fclose(fp);

  if ((tgaheader.imagewidth&(tgaheader.imagewidth-1))!=0)
    return;
  if ((tgaheader.imageheight&(tgaheader.imageheight-1))!=0)
    return;

  texture[texturenum].sizex=tgaheader.imagewidth;
  texture[texturenum].sizey=tgaheader.imageheight;
  texture[texturenum].mipmaplevels=1;
  texture[texturenum].format=GL_RGBA;
  texture[texturenum].alphamap=0;
  texture[texturenum].normalmap=0;
  texture[texturenum].glossmap=0;
  texture[texturenum].wraps=wraps;
  texture[texturenum].wrapt=wrapt;
  texture[texturenum].magfilter=magfilter;
  texture[texturenum].minfilter=minfilter;

  free(texture[texturenum].rgba[0]);
  texture[texturenum].rgba[0]=(unsigned int *) malloc(texture[texturenum].sizex*texture[texturenum].sizey*4);

  for (count=0;count<texture[texturenum].sizey;count++)
  for (count2=0;count2<texture[texturenum].sizex;count2++)
    {
    texture[texturenum].rgba[0][count*texture[texturenum].sizex+count2]=imagedata[count*tgaheader.imagewidth+count2];
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    if ((texture[texturenum].rgba[0][count*texture[texturenum].sizex+count2]>>24)!=255)
      texture[texturenum].alphamap=1;
#else
    if ((texture[texturenum].rgba[0][count*texture[texturenum].sizex+count2]&255)!=255)
      texture[texturenum].alphamap=1;
#endif
    }

  free(imagedata);

  if (mipmap)
    generatemipmap(texturenum);
  setuptexture(texturenum);
  }

void loadtexturetgapartial(int texturenum,char *filename,int startx,int starty,int sizex,int sizey)
  {
  int count,count2;
  unsigned int red,green,blue,alpha;
  int changeddir;
  unsigned char origin;
  struct TGAHEADER tgaheader;
  unsigned int imagedata[1024*1024];
  FILE *fp;

  if (strcmp(lasttextureloaded,filename)!=0)
    {
    changeddir=chdir("texture");
  
    if ((fp=fopen(filename,"rb"))==NULL)
      {
#ifdef DEBUG
      printf("Texture Load Failed: %d\n",texturenum);
#endif

      if (changeddir==0)
        chdir("..");
      return;
      }
  
    fseek(fp,2,SEEK_CUR);
    fread2(&tgaheader.imagetypecode,1,1,fp);
    if (tgaheader.imagetypecode!=2 && tgaheader.imagetypecode!=3)
      {
#ifdef DEBUG
      printf("Texture Bad Format: %d\n",texturenum);
#endif
  
      fclose(fp);
  
      if (changeddir==0)
        chdir("..");
      return;
      }
  
    fseek(fp,9,SEEK_CUR);
    fread2(&tgaheader.imagewidth,2,1,fp);
    fread2(&tgaheader.imageheight,2,1,fp);
    fread2(&tgaheader.pixeldepth,1,1,fp);
    fread2(&origin,1,1,fp);
    origin=(origin>>4)&3;
  
    for (count=0;count<tgaheader.imageheight;count++)
    for (count2=0;count2<tgaheader.imagewidth;count2++)
      {
      blue=(unsigned int)fgetc(fp);
      green=(unsigned int)fgetc(fp);
      red=(unsigned int)fgetc(fp);
      if (tgaheader.pixeldepth==32)
        alpha=(unsigned int)fgetc(fp);
      else
        alpha=255;
  
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    if (origin==0)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+count2]=(alpha<<24)+(blue<<16)+(green<<8)+red;
    if (origin==1)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(alpha<<24)+(blue<<16)+(green<<8)+red;
    if (origin==2)
      imagedata[count*tgaheader.imagewidth+count2]=(alpha<<24)+(blue<<16)+(green<<8)+red;
    if (origin==3)
      imagedata[count*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(alpha<<24)+(blue<<16)+(green<<8)+red;
#else
    if (origin==0)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+count2]=(red<<24)+(green<<16)+(blue<<8)+alpha;
    if (origin==1)
      imagedata[(tgaheader.imageheight-1-count)*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(red<<24)+(green<<16)+(blue<<8)+alpha;
    if (origin==2)
      imagedata[count*tgaheader.imagewidth+count2]=(red<<24)+(green<<16)+(blue<<8)+alpha;
    if (origin==3)
      imagedata[count*tgaheader.imagewidth+(tgaheader.imagewidth-1-count2)]=(red<<24)+(green<<16)+(blue<<8)+alpha;
#endif
    }
  
    fclose(fp);
  
    if (changeddir==0)
      chdir("..");

    strcpy(lasttextureloaded,filename);
    }

  texture[texturenum].sizex=sizex;
  texture[texturenum].sizey=sizey;
  texture[texturenum].mipmaplevels=1;
  texture[texturenum].format=GL_RGBA;
  texture[texturenum].alphamap=0;
  texture[texturenum].normalmap=0;
  texture[texturenum].glossmap=0;
  texture[texturenum].wraps=GL_CLAMP;
  texture[texturenum].wrapt=GL_CLAMP;
  texture[texturenum].magfilter=GL_NEAREST;
  texture[texturenum].minfilter=GL_NEAREST;

  free(texture[texturenum].rgba[0]);
  texture[texturenum].rgba[0]=(unsigned int *) malloc(texture[texturenum].sizex*texture[texturenum].sizey*4);

  for (count=0;count<texture[texturenum].sizey;count++)
  if (count<tgaheader.imageheight)
  for (count2=0;count2<texture[texturenum].sizex;count2++)
  if (count2<tgaheader.imagewidth)
    {
    texture[texturenum].rgba[0][count*texture[texturenum].sizex+count2]=imagedata[(starty+count)*tgaheader.imagewidth+(startx+count2)];
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    if ((texture[texturenum].rgba[0][count*texture[texturenum].sizex+count2]>>24)!=255)
      texture[texturenum].alphamap=1;
#else
    if ((texture[texturenum].rgba[0][count*texture[texturenum].sizex+count2]&255)!=255)
      texture[texturenum].alphamap=1;
#endif
    }

  setuptexture(texturenum);
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
