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

#include "glext.h"

#include <string.h>
#include <SDL_video.h>
#include "sdl/platform.h"

struct GLEXT glext;

void loadglextentions(void)
  {
  char *ext;
  char *glversion;

  glversion=(char *) glGetString(GL_VERSION);
  ext=(char *) glGetString(GL_EXTENSIONS);
#ifdef WINDOZE
  if (strstr(ext,"GL_ARB_multitexture")!=NULL || SDL_GL_GetProcAddress("glActiveTextureARB")!=NULL)
    glext.multitexture=1;
  else if (((void *) SDL_GL_GetProcAddress("glActiveTexture"))!=NULL)
    glext.multitexture=1;
#else
  glext.multitexture=1;
#endif
  if (strstr(ext,"GL_ARB_texture_env_dot3")!=NULL || (glversion[0]>='2' || glversion[2]>='3'))
    glext.texture_env_dot3=1;
  }
