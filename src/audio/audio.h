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

#ifndef __GISH_AUDIO_AUDIO_H__
#define __GISH_AUDIO_AUDIO_H__

#include "sdl/platform.h"
#ifndef THINKSTUPID
  #include <AL/al.h>
  #include <AL/alc.h>
#else
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#endif
#include <vorbis/vorbisfile.h>

#define OGGBUFFERSIZE 4096*8

void setupaudio(void);
int updateogg(void);
int streamogg(int buffernum);
void shutdownaudio(void);
void loadwav(int buffernum,char *filename);

extern int soundenabled;
extern ALuint soundbuffer[64];
extern int bufferloaded[64];

extern OggVorbis_File oggstream[2];

extern vorbis_info *vorbisinfo;

extern ALenum oggformat;
extern ALuint oggsource;
extern ALuint oggbuffer[2];

#endif
