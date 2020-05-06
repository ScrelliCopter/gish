#ifndef __GISH_SDL_PLATFORM_H__
#define __GISH_SDL_PLATFORM_H__

#include "config.h"

#ifdef _WIN32
  #define WINDOZE 1
#elif defined __APPLE__
  #define THINKSTUPID 1
#else
  #define DETLEF 1
#endif

#ifdef WINDOZE
  #define INLINE _inline
#else
  #define INLINE inline
#endif

#ifdef DETLEF
  #include <unistd.h>
#endif

#endif//__GISH_SDL_PLATFORM_H__
