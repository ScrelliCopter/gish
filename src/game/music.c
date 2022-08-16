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

#include "music.h"

#include <string.h>
#include <stdint.h>
#include <physfs.h>
#include "sdl/file.h"
#include "sdl/platform.h"
#include "audio/audio.h"
#include "game.h"
#include "config.h"
#include "options.h"
#include <unistd.h>

struct OGGMEMORYFILE
  {
  char *data;
  int64_t datasize;
  int dataread;
  };
struct OGGMEMORYFILE oggmemoryfile[16];
ov_callbacks vorbiscallbacks;

void checkmusic(void)
  {
  int count,count2;
  int queued;
  float vec[3];

  if (!soundenabled)
    return;
  if (!config.sound)
    return;
  if (!option.sound)
    return;

  if (!option.music)
    game.songnum=-1;

  if (game.songnum>8)
    game.songnum=0;

#ifdef DEMO
  if (game.songnum>0 && game.songnum<8)
    game.songnum=0;
#endif

  if (game.songnum!=game.currentsongnum)
    {
    if (game.currentsongnum!=-1)
      {
      alSourceStop(oggsource);

      alGetSourcei(oggsource,AL_BUFFERS_QUEUED,&queued);

      while (queued>0)
        {
        alSourceUnqueueBuffers(oggsource,1,&count);

        queued--;
        }

      ov_clear(&oggstream[0]);
      }

    if (game.songnum!=-1)
      {
      vorbiscallbacks.read_func=vorbisread;
      vorbiscallbacks.seek_func=vorbisseek;
      vorbiscallbacks.close_func=vorbisclose;
      vorbiscallbacks.tell_func=vorbistell;

      oggmemoryfile[game.songnum].dataread=0;
      if (ov_open_callbacks(&oggmemoryfile[game.songnum],&oggstream[0],NULL,0,vorbiscallbacks)>=0)
        {
        vorbisinfo=ov_info(&oggstream[0],-1);

        if (vorbisinfo->channels==1)
          oggformat=AL_FORMAT_MONO16;
        else
          oggformat=AL_FORMAT_STEREO16;

        streamogg(oggbuffer[0]);
        streamogg(oggbuffer[1]);

        alSourceQueueBuffers(oggsource,2,oggbuffer);

        vec[0]=0.0f;
        vec[1]=0.0f;
        vec[2]=0.0f;

        alSource3f(oggsource,AL_POSITION,vec[0],vec[1],vec[2]);
        alSource3f(oggsource,AL_VELOCITY,0.0f,0.0f,0.0f);
        alSourcef(oggsource,AL_REFERENCE_DISTANCE,10.0f);
        alSourcef(oggsource,AL_GAIN,0.3f*option.musicvolume);
        alSourcei(oggsource,AL_SOURCE_RELATIVE,AL_TRUE);

        alSourcePlay(oggsource);

        updateogg();

        alSourcePlay(oggsource);

        game.currentsongnum=game.songnum;
        }
      else
        game.currentsongnum=-1;
      }
    else
      {
      game.currentsongnum=game.songnum;
      }
    }

  if (game.currentsongnum!=-1)
    {
    alGetSourcei(oggsource,AL_SOURCE_STATE,&count);
    if (count!=AL_PLAYING)
      alSourcePlay(oggsource);
    }
  }

void loadogg(int index,const char *filename)
  {
  PHYSFS_file *fp;

  if ((fp=PHYSFS_openRead(filename))!=NULL)
    {
    oggmemoryfile[index].datasize=PHYSFS_fileLength(fp);;
    oggmemoryfile[index].dataread=0;
    oggmemoryfile[index].data=(char *)malloc((size_t)oggmemoryfile[index].datasize);
    PHYSFS_readBytes(fp,oggmemoryfile[index].data,(PHYSFS_uint64)oggmemoryfile[index].datasize);

    PHYSFS_close(fp);
    }
  }

void loadoggs(void)
  {
  loadogg(0,"music/sewer.ogg");
#ifndef DEMO
  loadogg(1,"music/cave.ogg");
  loadogg(2,"music/hell.ogg");
  loadogg(3,"music/egypt.ogg");
  loadogg(4,"music/church.ogg");
  loadogg(5,"music/boss.ogg");
  loadogg(6,"data/async.dat");
  loadogg(7,"music/versus.ogg");
#endif
  loadogg(8,"music/intro.ogg");
  }

size_t vorbisread(void *ptr,size_t bytesize,size_t sizetoread,void *datasource)
  {
  int actualsizetoread;
  int spacetoeof;
  struct OGGMEMORYFILE *vorbisdata;

  vorbisdata=(struct OGGMEMORYFILE *)datasource;

  spacetoeof=vorbisdata->datasize-vorbisdata->dataread;

  if (sizetoread*bytesize<spacetoeof)
    actualsizetoread=sizetoread*bytesize;
  else
    actualsizetoread=spacetoeof;

  if (actualsizetoread>0)
    {
    memcpy(ptr,(char *)vorbisdata->data+vorbisdata->dataread,actualsizetoread);
    vorbisdata->dataread+=actualsizetoread;
    }

  return(actualsizetoread);
  }

int vorbisseek(void *datasource,ogg_int64_t offset,int whence)
  {
  int spacetoeof;
  ogg_int64_t actualoffset;
  struct OGGMEMORYFILE *vorbisdata;

  vorbisdata=(struct OGGMEMORYFILE *)datasource;

  if (whence==SEEK_SET)
    {
    if (offset<vorbisdata->datasize)
      actualoffset=offset;
    else
      actualoffset=vorbisdata->datasize;
    vorbisdata->dataread=(int)actualoffset;
    }
  if (whence==SEEK_CUR)
    {
    spacetoeof=vorbisdata->datasize-vorbisdata->dataread;

    if (offset<spacetoeof)
      actualoffset=offset;
    else
      actualoffset=spacetoeof;
    vorbisdata->dataread+=(int)actualoffset;
    }
  if (whence==SEEK_END)
    {
    vorbisdata->dataread=vorbisdata->datasize+1;
    }
  return(0);
  }

int vorbisclose(void *datasource)
  {
  return(1);
  }

long vorbistell(void *datasource)
  {
  struct OGGMEMORYFILE *vorbisdata;

  vorbisdata=(struct OGGMEMORYFILE *)datasource;

  return(vorbisdata->dataread);
  }
