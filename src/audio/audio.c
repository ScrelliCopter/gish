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

#include "audio.h"

#include <SDL.h>
#include <physfs.h>
#include "sdl/physfsrwops.h"
#include "sdl/platform.h"
#include "game/game.h"
#include "game/audio.h"
#include "game/options.h"
#include <unistd.h>

ALCcontext *alcontext;
ALCdevice *aldevice;

int soundenabled;
ALuint soundbuffer[64];
int bufferloaded[64];

OggVorbis_File oggstream[2];
vorbis_info *vorbisinfo;

ALenum oggformat;
char oggdata[OGGBUFFERSIZE];

ALuint oggsource;
ALuint oggbuffer[2];

void setupaudio(void)
  {
  int count;
  ALsizei size,freq;
  ALenum format;
  ALvoid *data;
  ALboolean loop;
  int changeddir;

  aldevice=alcOpenDevice(NULL);
  if (aldevice!=NULL)
    {
    alcontext=alcCreateContext(aldevice,NULL);
    if (alcontext!=NULL)
      {
      alcMakeContextCurrent(alcontext);
      soundenabled=1;
      }
    }

  if (!soundenabled)
    return;

  for (count=0;count<2;count++)
    alGenBuffers(1,&oggbuffer[count]);
  for (count=0;count<30;count++)
    alGenBuffers(1,&soundbuffer[count]);

  loadwav(0,"sound/blockbreak.wav");
  loadwav(1,"sound/rockhit.wav");
  loadwav(2,"sound/fleshhit.wav");
  loadwav(3,"sound/ropebreak.wav");
  loadwav(4,"sound/chainbreak.wav");
  loadwav(5,"sound/gishhit.wav");
  loadwav(6,"sound/rockfriction.wav");
  loadwav(7,"sound/squish.wav");
  loadwav(8,"sound/secrets1.wav");
  loadwav(9,"sound/secrets2.wav");
  loadwav(10,"sound/amber.wav");
  loadwav(11,"sound/nibattack.wav");
  loadwav(12,"sound/visattack.wav");
  loadwav(13,"sound/bobattack.wav");
  loadwav(14,"sound/switch.wav");
  loadwav(15,"sound/points.wav");
  loadwav(16,"sound/gishhurt.wav");
  loadwav(17,"sound/splash.wav");
  loadwav(18,"sound/lava.wav");
  loadwav(19,"sound/necksnap.wav");
  loadwav(20,"sound/tarball.wav");

#ifndef DEMO
  loadwav(21,"data/cubemap.dat");
  loadwav(22,"data/specular.dat");
  loadwav(23,"data/stencil.dat");
  loadwav(24,"data/pixel.dat");
#endif

  alGenSources(1,&oggsource);

  for (count=0;count<12;count++)
    alGenSources(1,&sound[count].alname);

  //alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
  }

int updateogg(void)
  {
  int processed;
  int buffernum;
  int active;

  if (!soundenabled)
    return(1);
  if (!option.music)
    return(1);

  active=1;

  alGetSourcei(oggsource,AL_BUFFERS_PROCESSED,&processed);

  while (processed>0)
    {
    alSourceUnqueueBuffers(oggsource,1,&buffernum);

    active=streamogg(buffernum);

    if (active)
      alSourceQueueBuffers(oggsource,1,&buffernum);

    processed--;
    }
  if (!active)
    {
    game.songnum=-1;
    /*
    if (level.gametype<10)
      {
      game.songnum++;
      if (game.songnum>3)
        game.songnum=0;
      }
    */
    }
  return(active);
  }

int streamogg(int buffernum)
  {
  int size;
  int section;
  int result;
  int count;
  char temp;

  size=0;
  while (size<OGGBUFFERSIZE)
    {
    result=ov_read(&oggstream[0],oggdata+size,OGGBUFFERSIZE-size,0,2,1,&section);

    if (result>0)
      size+=result;
    else
      goto streamoggbypass;
    }
  streamoggbypass:;

  if (size==0)
    return(0);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  for (count=0;count<size/2;count++)
    {
    temp=oggdata[count*2];
    oggdata[count*2]=oggdata[count*2+1];
    oggdata[count*2+1]=temp;
    }
#endif

  alBufferData(buffernum,oggformat,oggdata,size,vorbisinfo->rate);

  return(1);
  }

void shutdownaudio(void)
  {
  alcontext=alcGetCurrentContext();
  aldevice=alcGetContextsDevice(alcontext);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(alcontext);
  alcCloseDevice(aldevice);

  //ov_clear(&oggstream[0]);
  }

void loadwav(int buffernum,char *filename)
  {
  int count;
  SDL_AudioSpec wavspec;
  unsigned int wavlength;
  unsigned char *wavbuffer;
  unsigned char temp;
  ALenum format;

  PHYSFS_openRead(filename);
  if (SDL_LoadWAV_RW(PHYSFSRWOPS_openRead(filename),1,&wavspec,&wavbuffer,&wavlength))
    {
    if (wavspec.channels==1)
      {
      if (wavspec.format==AUDIO_U8 || wavspec.format==AUDIO_S8)
        format=AL_FORMAT_MONO8;
      else
        format=AL_FORMAT_MONO16;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
      if (format==AL_FORMAT_MONO16)
        {
        for (count=0;count<wavlength/2;count++)
          {
          temp=wavbuffer[count*2+0];
          wavbuffer[count*2+0]=wavbuffer[count*2+1];
          wavbuffer[count*2+1]=temp;
          }
        }
#endif

      alBufferData(soundbuffer[buffernum],format,wavbuffer,wavlength,wavspec.freq);
      bufferloaded[buffernum]=1;
      }
    SDL_FreeWAV(wavbuffer);
    }
  }

