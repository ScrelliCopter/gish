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

#include "level.h"

#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <SDL_endian.h>
#include <sdl/file.h>
#include <physfs.h>
#include <sdl/platform.h>
#include <video/texture.h>
#include <math/vector.h>
#include "game.h"
#include "object.h"
#include "block.h"
#include <physics/particle.h>
#include "physics.h"
#include "mainmenu.h"
#include "editor.h"
#include "socket.h"

unsigned int cryptdata[1048576];
int textureused[512];
struct COLLISION collision;
int levelblocknum;
float levelfriction;
struct LEVEL level;

int lineintersectlevel(float *intersectpoint,float *normal,float *scale,float *startpoint,float *endpoint)
  {
  int count,count2,count3;
  int blocknum;
  int min[2],max[2];
  float vec[3],vec2[3],vec3[3],vec4[3];
  float intersectpointtemp[3];
  float normaltemp[3];
  float scaletemp;

  for (count=0;count<2;count++)
    {
    if (startpoint[count]<endpoint[count])
      {
      min[count]=startpoint[count];
      max[count]=endpoint[count];
      }
    else
      {
      min[count]=endpoint[count];
      max[count]=startpoint[count];
      }
    if (min[count]<0)
      min[count]=0;
    if (max[count]<0)
      max[count]=0;
    if (min[count]>255)
      min[count]=255;
    if (max[count]>255)
      max[count]=255;
    }

  *scale=1.0f;

  for (count=min[1];count<=max[1];count++)
  for (count2=min[0];count2<=max[0];count2++)
    {
    blocknum=level.grid[count][count2];
    if (level.gridmod[count][count2]!=0)
      blocknum=0;

    for (count3=0;count3<block[blocknum].numoflines;count3++)
    if (((level.gridflags[count][count2]>>count3)&1)==0)
      {
      vec[0]=(float)count2+block[blocknum].line[count3][0];
      vec[1]=(float)count+block[blocknum].line[count3][1];
      vec[2]=0.0f;
      vec2[0]=(float)count2+block[blocknum].line[count3][2];
      vec2[1]=(float)count+block[blocknum].line[count3][3];
      vec2[2]=0.0f;
      if (lineintersectline2(intersectpointtemp,normaltemp,&scaletemp,startpoint,endpoint,vec,vec2))
      if (scaletemp<*scale)
        {
        collision.blocknum=blocknum;
        collision.blockx=count2;
        collision.blocky=count;
        collision.friction=block[blocknum].friction;
        copyvector(intersectpoint,intersectpointtemp);
        copyvector(normal,normaltemp);
        *scale=scaletemp;
        /*
        subtractvectors(vec3,endpoint,vec);
        subtractvectors(vec4,vec2,vec);
        if (dotproduct(vec3,vec4)>0.0f)
          {
          subtractvectors(vec3,endpoint,vec2);
          subtractvectors(vec4,vec,vec2);
          if (dotproduct(vec3,vec4)>0.0f)
            {
            copyvector(intersectpoint,intersectpointtemp);
            copyvector(normal,normaltemp);
            *scale=scaletemp;
            }
          }
        */
        }
      }
    }
  if (*scale<1.0f)
    return(1);

  return(0);
  }

void savelevel(char *filename)
  {
  int count,count2,count3;
  int blocknum;
  int changeddir;
  int version;
  FILE *fp;

  for (count=0;count<256;count++)
    textureused[count]=1;

  for (count=0;count<256;count++)
  for (count2=0;count2<256;count2++)
    {
    blocknum=level.backgrid[count][count2];
    textureused[blocknum]=1;
    if (block[blocknum].animation!=0)
      for (count3=1;count3<=block[blocknum].animation;count3++)
        textureused[blocknum+count3]=1;

    blocknum=level.grid[count][count2];
    textureused[blocknum]=1;
    if (block[blocknum].animation!=0)
      for (count3=1;count3<=block[blocknum].animation;count3++)
        textureused[blocknum+count3]=1;

    blocknum=level.foregrid[count][count2];
    textureused[blocknum]=1;
    if (block[blocknum].animation!=0)
      for (count3=1;count3<=block[blocknum].animation;count3++)
        textureused[blocknum+count3]=1;
    }

  /*
  for (count=0;count<256;count++)
  if (!textureused[count])
    texture[count].sizex=0;
  */

  changeddir=chdir("level");

  if ((fp=fopen(filename,"wb"))!=NULL)
    {
    version=10;

    fwrite2(&version,4,1,fp);
    fwrite2(level.background,1,32,fp);
    fwrite2(&level.tileset,4,1,fp);
    fwrite2(&level.gametype,4,1,fp);
    fwrite2(&level.time,4,1,fp);
    fwrite2(level.area,4,64*4,fp);

    fwrite2(level.backgrid,1,256*256,fp);

    fwrite2(level.grid,1,256*256,fp);

    fwrite2(level.foregrid,1,256*256,fp);

    fwrite2(level.startposition,4,3,fp);
    fwrite2(level.ambient,4,12,fp);
    fwrite2(&level.numofobjects,4,1,fp);
    for (count=0;count<level.numofobjects;count++)
      {
      fwrite2(&level.object[count].type,4,1,fp);
      fwrite2(&level.object[count].texturenum,4,1,fp);
      fwrite2(&level.object[count].link,4,1,fp);
      fwrite2(level.object[count].position,4,3,fp);
      fwrite2(&level.object[count].angle,4,1,fp);
      fwrite2(level.object[count].size,4,2,fp);
      fwrite2(&level.object[count].mass,4,1,fp);
      fwrite2(&level.object[count].friction,4,1,fp);
      fwrite2(&level.object[count].lighttype,4,1,fp);
      fwrite2(level.object[count].lightcolor,4,3,fp);
      fwrite2(&level.object[count].lightintensity,4,1,fp);
      fwrite2(&level.object[count].ai,4,1,fp);
      }
    fwrite2(&level.numofropes,4,1,fp);
    for (count=0;count<level.numofropes;count++)
      {
      fwrite2(&level.rope[count].type,4,1,fp);
      fwrite2(&level.rope[count].texturenum,4,1,fp);
      fwrite2(&level.rope[count].obj1,4,1,fp);
      fwrite2(&level.rope[count].obj1part,4,1,fp);
      fwrite2(&level.rope[count].obj2,4,1,fp);
      fwrite2(&level.rope[count].obj2part,4,1,fp);
      }

    for (count=1;count<251;count++)
      {
      if (textureused[count])
        {
        fwrite2(&texture[count].sizex,4,1,fp);
        if (texture[count].sizex!=0)
          {
          fwrite2(&texture[count].sizey,4,1,fp);
          fwrite2(&texture[count].magfilter,4,1,fp);
          fwrite2(&texture[count].minfilter,4,1,fp);
          fwrite2(texture[count].rgba[0],4,texture[count].sizex*texture[count].sizey,fp);
          }
        }
      else
        {
        count2=0;
        fwrite2(&count2,4,1,fp);
        }

      fwrite2(&block[count].numoflines,4,1,fp);
      for (count2=0;count2<block[count].numoflines;count2++)
        fwrite2(block[count].line[count2],4,8,fp);
      fwrite2(&block[count].friction,4,1,fp);
      fwrite2(&block[count].breakpoint,4,1,fp);
      fwrite2(&block[count].middamage,4,1,fp);
      fwrite2(&block[count].foredamage,4,1,fp);
      fwrite2(&block[count].density,4,1,fp);
      fwrite2(&block[count].drag,4,1,fp);
      fwrite2(&block[count].animation,4,1,fp);
      fwrite2(&block[count].animationspeed,4,1,fp);
      }

    fclose(fp);
    }

  if (changeddir==0)
    chdir("..");
  }

void loadlevel(char *filename)
  {
  int count,count2;
  int changeddir;
  int version;
  unsigned int x,y;
  char fullpath[256];
  PHYSFS_file *fp;

  x=0x17AF2E03;

  snprintf(fullpath,sizeof(fullpath),"level/%s",filename);
  if ((fp=PHYSFS_openRead(fullpath))!=NULL)
    {
    PHYSFS_readSLE32(fp,&version);

    if (version==9||version==10)
      {
      strcpy(editor.filename,filename);

      PHYSFS_readBytes(fp,level.background,32);
      PHYSFS_readSLE32(fp,&level.tileset);
      PHYSFS_readSLE32(fp,&level.gametype);
      PHYSFS_readSLE32(fp,&level.time);
      for (int i=0;i<64;i++)
      for (int j=0;j<4;j++)
        PHYSFS_readSLE32(fp,&level.area[i][j]);

      if (version==9)
        {
        PHYSFS_readBytes(fp,level.backgrid,256*256);
        memcpy(cryptdata,level.backgrid,256*256);
        decryptdata(x,256*256/4);
        memcpy(level.backgrid,cryptdata,256*256);

        PHYSFS_readBytes(fp,level.grid,256*256);
        memcpy(cryptdata,level.grid,256*256);
        decryptdata(x,256*256/4);
        memcpy(level.grid,cryptdata,256*256);

        PHYSFS_readBytes(fp,level.foregrid,256*256);
        memcpy(cryptdata,level.foregrid,256*256);
        decryptdata(x,256*256/4);
        memcpy(level.foregrid,cryptdata,256*256);
        }
      else
        {
        PHYSFS_readBytes(fp,level.backgrid,256*256);
        PHYSFS_readBytes(fp,level.grid,256*256);
        PHYSFS_readBytes(fp,level.foregrid,256*256);
        }

      for (int i=0;i<3;i++)
        PHYSFS_readSLE32(fp,(void*)&level.startposition[i]);
      for (int i=0;i<4;i++)
      for (int j=0;j<3;j++)
        PHYSFS_readSLE32(fp,(void*)&level.ambient[i][j]);

      PHYSFS_readSLE32(fp,&level.numofobjects);
      if (level.numofobjects<0 || level.numofobjects>=256)
        {
        PHYSFS_close(fp);
        return;
        }
      for (count=0;count<level.numofobjects;count++)
        {
        PHYSFS_readSLE32(fp,&level.object[count].type);
        PHYSFS_readSLE32(fp,&level.object[count].texturenum);
        PHYSFS_readSLE32(fp,&level.object[count].link);
        for (int i=0;i<3;i++)
          PHYSFS_readSLE32(fp,(void*)&level.object[count].position[i]);
        PHYSFS_readSLE32(fp,(void*)&level.object[count].angle);
        for (int i=0;i<2;i++)
          PHYSFS_readSLE32(fp,(void*)&level.object[count].size[i]);
        PHYSFS_readSLE32(fp,(void*)&level.object[count].mass);
        PHYSFS_readSLE32(fp,(void*)&level.object[count].friction);
        PHYSFS_readSLE32(fp,&level.object[count].lighttype);
        for (int i=0;i<3;i++)
          PHYSFS_readSLE32(fp,(void*)&level.object[count].lightcolor[i]);
        PHYSFS_readSLE32(fp,(void*)&level.object[count].lightintensity);
        PHYSFS_readSLE32(fp,&level.object[count].ai);
        }

      PHYSFS_readSLE32(fp,&level.numofropes);
      if (level.numofropes<0 || level.numofropes>=1024)
        {
        PHYSFS_close(fp);
        return;
        }
      for (count=0;count<level.numofropes;count++)
        {
        PHYSFS_readSLE32(fp,&level.rope[count].type);
        PHYSFS_readSLE32(fp,&level.rope[count].texturenum);
        PHYSFS_readSLE32(fp,&level.rope[count].obj1);
        PHYSFS_readSLE32(fp,&level.rope[count].obj1part);
        PHYSFS_readSLE32(fp,&level.rope[count].obj2);
        PHYSFS_readSLE32(fp,&level.rope[count].obj2part);
        }

      for (count=1;count<251;count++)
        {
        PHYSFS_readSLE32(fp,&texture[count].sizex);
        if (texture[count].sizex<0 || texture[count].sizex>=1024)
          {
          PHYSFS_close(fp);
          return;
          }
        if (texture[count].sizex!=0)
          {
          PHYSFS_readSLE32(fp,&texture[count].sizey);
          PHYSFS_readSLE32(fp,&texture[count].magfilter);
          PHYSFS_readSLE32(fp,&texture[count].minfilter);
          if (texture[count].rgba[0])
            free(texture[count].rgba[0]);
          texture[count].rgba[0]=(unsigned int *)malloc((size_t)texture[count].sizex*texture[count].sizey*4);
          PHYSFS_readBytes(fp,texture[count].rgba[0],(size_t)texture[count].sizex*texture[count].sizey*4);
          if (version==9)
            {
            memcpy(cryptdata,texture[count].rgba[0],(size_t)texture[count].sizex*texture[count].sizey*4);
            decryptdata(x,4*texture[count].sizex*texture[count].sizey/4);
            memcpy(texture[count].rgba[0],cryptdata,(size_t)texture[count].sizex*texture[count].sizey*4);
            }

          texture[count].mipmaplevels=1;
          texture[count].format=GL_RGBA;
          texture[count].alphamap=0;
          texture[count].normalmap=0;
          texture[count].glossmap=0;
          texture[count].wraps=GL_CLAMP_TO_EDGE;
          texture[count].wrapt=GL_CLAMP_TO_EDGE;
          texture[count].magfilter=GL_LINEAR;
          texture[count].minfilter=GL_LINEAR;

          if ((texture[count].sizex&(texture[count].sizex-1))==0)
          if ((texture[count].sizey&(texture[count].sizey-1))==0)
            setuptexture(count);
          }
  
        PHYSFS_readSLE32(fp,&block[count].numoflines);
        if (block[count].numoflines<0 || block[count].numoflines>=64)
          {
          PHYSFS_close(fp);
          return;
          }
        for (int i=0;i<block[count].numoflines;i++)
        for (int j=0;j<8;j++)
          PHYSFS_readSLE32(fp,(void*)&block[count].line[i][j]);
        PHYSFS_readSLE32(fp,(void*)&block[count].friction);
        PHYSFS_readSLE32(fp,(void*)&block[count].breakpoint);
        PHYSFS_readSLE32(fp,&block[count].middamage);
        PHYSFS_readSLE32(fp,&block[count].foredamage);
        PHYSFS_readSLE32(fp,(void*)&block[count].density);
        PHYSFS_readSLE32(fp,(void*)&block[count].drag);
        PHYSFS_readSLE32(fp,&block[count].animation);
        PHYSFS_readSLE32(fp,&block[count].animationspeed);
        }
      }

    PHYSFS_close(fp);
    }

  if (version<7)
    loadleveltextures();
  else
    {
    lasttextureloaded[0]=0;
    if (level.background[0]!=0)
      snprintf(fullpath,sizeof(fullpath),"texture/%s",level.background);
      loadbackground(660,fullpath);
    }

  loadtexturetga(251,"texture/oneup.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(252,"texture/tarball.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(253,"texture/amber1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(254,"texture/amber2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(255,"texture/amber3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  }

void createlevel(void)
  {
  int count,count2;

  memset(&level,0,sizeof(level));

  for (count=0;count<256;count++)
  for (count2=0;count2<256;count2++)
    level.backgrid[count][count2]=1;
  for (count=0;count<256;count++)
  for (count2=0;count2<256;count2++)
    level.grid[count][count2]=2;
  for (count=128-4;count<128+4;count++)
  for (count2=128-8;count2<128+8;count2++)
    level.grid[count][count2]=0;

  level.ambient[0][0]=0.25f;
  level.ambient[0][1]=0.25f;
  level.ambient[0][2]=0.25f;

  level.ambient[1][0]=0.5f;
  level.ambient[1][1]=0.5f;
  level.ambient[1][2]=0.5f;

  level.ambient[2][0]=0.5f;
  level.ambient[2][1]=0.5f;
  level.ambient[2][2]=0.5f;

  level.ambient[3][0]=1.0f;
  level.ambient[3][1]=1.0f;
  level.ambient[3][2]=1.0f;

  level.object[0].type=1;
  level.object[0].position[0]=127.5f;
  level.object[0].position[1]=127.5f;
  level.object[0].position[2]=0.0f;
  level.numofobjects=1;

  loadleveltextures();

  loadtexturetga(251,"texture/oneup.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(252,"texture/tarball.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(253,"texture/amber1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(254,"texture/amber2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(255,"texture/amber3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  editor.filename[0]=0;
  }

void getlevellines(int objectnum)
  {
  int count,count2,count3;
  int min[2],max[2];
  int blocknum;
  float vec[3];
  float normal[3];

  numoflevellines=0;

  for (count2=0;count2<2;count2++)
    {
    min[count2]=object[objectnum].position[count2];
    max[count2]=object[objectnum].position[count2];
    }

  for (count=0;count<object[objectnum].numofparticles;count++)
    {
    for (count2=0;count2<2;count2++)
      {
      if (min[count2]>particle[object[objectnum].particle[count]].position[count2])
        min[count2]=particle[object[objectnum].particle[count]].position[count2];
      if (max[count2]<particle[object[objectnum].particle[count]].position[count2])
        max[count2]=particle[object[objectnum].particle[count]].position[count2];
      }
    }

  for (count=min[1];count<=max[1];count++)
  for (count2=min[0];count2<=max[0];count2++)
    {
    blocknum=level.grid[count][count2];
    if (level.gridmod[count][count2]!=0)
      blocknum=0;
    for (count3=0;count3<block[blocknum].numoflines;count3++)
    if (((level.gridflags[count][count2]>>count3)&1)==0)
      {
      vec[0]=(float)count2+(block[blocknum].line[count3][0]+block[blocknum].line[count3][2])*0.5f;
      vec[1]=(float)count+(block[blocknum].line[count3][1]+block[blocknum].line[count3][3])*0.5f;
      vec[2]=0.0f;
      vec[0]+=(block[blocknum].line[count3][3]-block[blocknum].line[count3][1])*0.5f;
      vec[1]-=(block[blocknum].line[count3][2]-block[blocknum].line[count3][0])*0.5f;

      normal[0]=-(block[blocknum].line[count3][3]-block[blocknum].line[count3][1]);
      normal[1]=(block[blocknum].line[count3][2]-block[blocknum].line[count3][0]);
      normal[2]=0.0f;
      normalizevector(normal,normal);

      levelline[numoflevellines].line[0][0]=vec[0];
      levelline[numoflevellines].line[0][1]=vec[1];
      levelline[numoflevellines].line[0][2]=0.0f;
      levelline[numoflevellines].line[1][0]=(float)count2+block[blocknum].line[count3][0];
      levelline[numoflevellines].line[1][1]=(float)count+block[blocknum].line[count3][1];
      levelline[numoflevellines].line[1][2]=0.0f;
      copyvector(levelline[numoflevellines].normal,normal);
      levelline[numoflevellines].friction=block[blocknum].friction;
      levelline[numoflevellines].blocknum=blocknum;
      levelline[numoflevellines].blockx=count2;
      levelline[numoflevellines].blocky=count;
      numoflevellines++;

      levelline[numoflevellines].line[0][0]=vec[0];
      levelline[numoflevellines].line[0][1]=vec[1];
      levelline[numoflevellines].line[0][2]=0.0f;
      levelline[numoflevellines].line[1][0]=(float)count2+block[blocknum].line[count3][2];
      levelline[numoflevellines].line[1][1]=(float)count+block[blocknum].line[count3][3];
      levelline[numoflevellines].line[1][2]=0.0f;
      copyvector(levelline[numoflevellines].normal,normal);
      levelline[numoflevellines].friction=block[blocknum].friction;
      levelline[numoflevellines].blocknum=blocknum;
      levelline[numoflevellines].blockx=count2;
      levelline[numoflevellines].blocky=count;
      numoflevellines++;
      }
    }
  }

void setuplevellines(int xstart,int ystart,int xend,int yend)
  {
  int count,count2,count3,count4;
  int blocknum,blocknum2;

  if (xstart<1)
    xstart=1;
  if (ystart<1)
    ystart=1;
  if (xend>254)
    xend=254;
  if (yend>254)
    yend=254;

  for (count=ystart;count<=yend;count++)
  for (count2=xstart;count2<=xend;count2++)
    {
    level.gridflags[count][count2]=0;

    blocknum=level.grid[count][count2];

    for (count3=0;count3<block[blocknum].numoflines;count3++)
      {
      if (level.gridmod[count-1][count2]==0)
        {
        blocknum2=level.grid[count-1][count2];
        for (count4=0;count4<block[blocknum2].numoflines;count4++)
        if (block[blocknum].line[count3][1]==0.0f)
        if (block[blocknum].line[count3][3]==0.0f)
        if (block[blocknum2].line[count4][1]==1.0f)
        if (block[blocknum2].line[count4][3]==1.0f)
        if (block[blocknum].line[count3][0]<=block[blocknum2].line[count4][2])
        if (block[blocknum].line[count3][2]>=block[blocknum2].line[count4][0])
          level.gridflags[count][count2]|=(1<<count3);
        }
      if (level.gridmod[count+1][count2]==0)
        {
        blocknum2=level.grid[count+1][count2];
        for (count4=0;count4<block[blocknum2].numoflines;count4++)
        if (block[blocknum].line[count3][1]==1.0f)
        if (block[blocknum].line[count3][3]==1.0f)
        if (block[blocknum2].line[count4][1]==0.0f)
        if (block[blocknum2].line[count4][3]==0.0f)
        if (block[blocknum].line[count3][0]>=block[blocknum2].line[count4][2])
        if (block[blocknum].line[count3][2]<=block[blocknum2].line[count4][0])
          level.gridflags[count][count2]|=(1<<count3);
        }
      if (level.gridmod[count][count2-1]==0)
        {
        blocknum2=level.grid[count][count2-1];
        for (count4=0;count4<block[blocknum2].numoflines;count4++)
        if (block[blocknum].line[count3][0]==0.0f)
        if (block[blocknum].line[count3][2]==0.0f)
        if (block[blocknum2].line[count4][0]==1.0f)
        if (block[blocknum2].line[count4][2]==1.0f)
        if (block[blocknum].line[count3][1]>=block[blocknum2].line[count4][3])
        if (block[blocknum].line[count3][3]<=block[blocknum2].line[count4][1])
          level.gridflags[count][count2]|=(1<<count3);
        }
      if (level.gridmod[count][count2+1]==0)
        {
        blocknum2=level.grid[count][count2+1];
        for (count4=0;count4<block[blocknum2].numoflines;count4++)
        if (block[blocknum].line[count3][0]==1.0f)
        if (block[blocknum].line[count3][2]==1.0f)
        if (block[blocknum2].line[count4][0]==0.0f)
        if (block[blocknum2].line[count4][2]==0.0f)
        if (block[blocknum].line[count3][1]<=block[blocknum2].line[count4][3])
        if (block[blocknum].line[count3][3]>=block[blocknum2].line[count4][1])
          level.gridflags[count][count2]|=(1<<count3);
        }

      /*
      if ((block[level.grid[count-1][count2]].flags&1)==1)
      if (level.gridmod[count-1][count2]==0)
        {
        if (block[blocknum].line[count3][1]==0.0f)
        if (block[blocknum].line[count3][3]==0.0f)
        if (block[blocknum].line[count3][0]>block[blocknum].line[count3][2])
          level.gridflags[count][count2]|=(1<<count3);
        }
      if ((block[level.grid[count][count2-1]].flags&2)==2)
      if (level.gridmod[count][count2-1]==0)
        {
        if (block[blocknum].line[count3][0]==0.0f)
        if (block[blocknum].line[count3][2]==0.0f)
        if (block[blocknum].line[count3][1]<block[blocknum].line[count3][3])
          level.gridflags[count][count2]|=(1<<count3);
        }
      if ((block[level.grid[count+1][count2]].flags&4)==4)
      if (level.gridmod[count+1][count2]==0)
        {
        if (block[blocknum].line[count3][1]==1.0f)
        if (block[blocknum].line[count3][3]==1.0f)
        if (block[blocknum].line[count3][0]<block[blocknum].line[count3][2])
          level.gridflags[count][count2]|=(1<<count3);
        }
      if ((block[level.grid[count][count2+1]].flags&8)==8)
      if (level.gridmod[count][count2+1]==0)
        {
        if (block[blocknum].line[count3][0]==1.0f)
        if (block[blocknum].line[count3][2]==1.0f)
        if (block[blocknum].line[count3][1]>block[blocknum].line[count3][3])
          level.gridflags[count][count2]|=(1<<count3);
        }
      */
      }
    if (level.gridmod[count][count2]!=0)
      level.gridflags[count][count2]=0xFFFFFFFF;
    }
  }

void loadleveltextures(void)
  {
  int count;
  char texfilename[256];

  lasttextureloaded[0]=0;
  loadbackground(660,"texture/bg.tga");

  for (count=1;count<251;count++)
    loadblock(count);
  for (count=0;count<251;count++)
    {
    snprintf(texfilename,sizeof(texfilename),"tile%02d/texture/text%03d.tga",level.tileset+1,count);
    if (game.levelnum!=6)
      loadtexturetga(count,texfilename,0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
    else
      loadtexturetga(count,texfilename,0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_NEAREST,GL_NEAREST);
    }
  }

int lineintersectline3(float *intersectpoint,float *normal,float *scale,float *startpoint,float *endpoint,float *vertex1,float *vertex2)
  {
  float vec[3],vec2[3];
  float dot1,dot2;

  normal[0]=vertex1[1]-vertex2[1];
  normal[1]=vertex2[0]-vertex1[0];
  normal[2]=0.0f;

  vec[0]=startpoint[0]-vertex1[0];
  vec[1]=startpoint[1]-vertex1[1];
  vec[2]=0.0f;

  vec2[0]=endpoint[0]-vertex1[0];
  vec2[1]=endpoint[1]-vertex1[1];
  vec2[2]=0.0f;

  dot1=dotproduct(normal,vec);
  dot2=dotproduct(normal,vec2);
  if (dot1>0.0f)
  if (dot2>0.0f)
    return(0);

  if (dot1<0.0f)
    return(0);

  normalizevector(normal,normal);

  vec[0]=vertex1[0]-startpoint[0];
  vec[1]=vertex1[1]-startpoint[1];
  vec[2]=0.0f;
  *scale=dotproduct(vec,normal);
  vec[0]=endpoint[0]-startpoint[0];
  vec[1]=endpoint[1]-startpoint[1];
  vec[2]=0.0f;
  *scale/=dotproduct(vec,normal);

  scaleaddvectors(intersectpoint,startpoint,vec,*scale);

  vec[0]=intersectpoint[0]-vertex1[0];
  vec[1]=intersectpoint[1]-vertex1[1];
  vec[2]=0.0f;

  vec2[0]=vertex2[0]-vertex1[0];
  vec2[1]=vertex2[1]-vertex1[1];
  vec2[2]=0.0f;

  if (dotproduct(vec,vec2)<0.0f)
    return(0);

  vec[0]=intersectpoint[0]-vertex2[0];
  vec[1]=intersectpoint[1]-vertex2[1];
  vec[2]=0.0f;

  vec2[0]=vertex1[0]-vertex2[0];
  vec2[1]=vertex1[1]-vertex2[1];
  vec2[2]=0.0f;

  if (dotproduct(vec,vec2)<0.0f)
    return(0);

  vec[0]=endpoint[0]-vertex1[0];
  vec[1]=endpoint[1]-vertex1[1];
  vec[2]=0.0f;

  vec2[0]=vertex2[0]-vertex1[0];
  vec2[1]=vertex2[1]-vertex1[1];
  vec2[2]=0.0f;

  if (dotproduct(vec,vec2)<0.0f)
    return(0);

  vec[0]=endpoint[0]-vertex2[0];
  vec[1]=endpoint[1]-vertex2[1];
  vec[2]=0.0f;

  vec2[0]=vertex1[0]-vertex2[0];
  vec2[1]=vertex1[1]-vertex2[1];
  vec2[2]=0.0f;

  if (dotproduct(vec,vec2)<0.0f)
    return(0);

  return(1);
  }


int pointintersectlevel(float *intersectpoint,float *normal,float *scale,float *point)
  {
  int count,count2,count3;
  int blocknum;
  int min[2],max[2];
  float vec[3],vec2[3],vec3[3];
  float intersectpointtemp[3];
  float normaltemp[3];
  float scaletemp;

  for (count=0;count<2;count++)
    {
    min[count]=point[count]-1.0f;
    max[count]=point[count]+1.0f;

    if (min[count]<0)
      min[count]=0;
    if (max[count]<0)
      max[count]=0;
    if (min[count]>255)
      min[count]=255;
    if (max[count]>255)
      max[count]=255;
    }

  *scale=0.0f;

  for (count=min[1];count<=max[1];count++)
  for (count2=min[0];count2<=max[0];count2++)
    {
    blocknum=level.grid[count][count2];
    if (level.gridmod[count][count2]!=0)
      blocknum=0;

    for (count3=0;count3<block[blocknum].numoflines;count3++)
    if (((level.gridflags[count][count2]>>count3)&1)==0)
      {
      vec[0]=(float)count2+block[blocknum].line[count3][0];
      vec[1]=(float)count+block[blocknum].line[count3][1];
      vec[2]=0.0f;
      vec2[0]=(float)count2+block[blocknum].line[count3][2];
      vec2[1]=(float)count+block[blocknum].line[count3][3];
      vec2[2]=0.0f;

      normaltemp[0]=vec[1]-vec2[1];
      normaltemp[1]=vec2[0]-vec[0];
      normaltemp[2]=0.0f;
      normalizevector(normaltemp,normaltemp);

      scaleaddvectors(vec3,point,normaltemp,0.25f);
      if (lineintersectline2(intersectpointtemp,normaltemp,&scaletemp,vec3,point,vec,vec2))
      if (scaletemp>*scale)
        {
        collision.blocknum=blocknum;
        collision.blockx=count2;
        collision.blocky=count;
        collision.friction=block[blocknum].friction;
        copyvector(intersectpoint,intersectpointtemp);
        copyvector(normal,normaltemp);
        *scale=scaletemp;
        }
      }
    }
  if (*scale>0.0f)
    return(1);

  return(0);
  }

void encryptdata(unsigned int code,unsigned int codepair,int cryptdatasize)
  {
  int count;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  byteswapdata(cryptdatasize);
#endif

  for (count=0;count<cryptdatasize;count++)
    {
    cryptdata[count]+=code;
    cryptdata[count]=rotint(cryptdata[count],(code&31));
    cryptdata[count]*=codepair;
    }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  byteswapdata(cryptdatasize);
#endif
  }

void decryptdata(unsigned int code,int cryptdatasize)
  {
  int count;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  byteswapdata(cryptdatasize);
#endif

  for (count=0;count<cryptdatasize;count++)
    {
    cryptdata[count]*=code;
    cryptdata[count]=rotint(cryptdata[count],(code&31));
    cryptdata[count]-=code;
    }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  byteswapdata(cryptdatasize);
#endif
  }

void byteswapdata(int cryptdatasize)
  {
  int count;
  unsigned int temp;

  for (count=0;count<cryptdatasize;count++)
    {
    temp=((cryptdata[count]&255)<<24)+(((cryptdata[count]>>8)&255)<<16);
    temp+=(((cryptdata[count]>>16)&255)<<8)+((cryptdata[count]>>24)&255);
    cryptdata[count]=temp;
    }
  }
