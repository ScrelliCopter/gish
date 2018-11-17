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

#ifndef __GISH_MATH_VECTOR_H__
#define __GISH_MATH_VECTOR_H__

#include <string.h>
#include <math.h>
#include <sdl/platform.h>

INLINE float dotproduct(float *vec,float *vec2)
  {
  return(vec[0]*vec2[0]+vec[1]*vec2[1]+vec[2]*vec2[2]);
  }

INLINE void crossproduct(float *result,float *vec,float *vec2)
  {
  result[0]=vec2[1]*vec[2]-vec2[2]*vec[1];
  result[1]=vec2[2]*vec[0]-vec2[0]*vec[2];
  result[2]=vec2[0]*vec[1]-vec2[1]*vec[0];
  }

INLINE float vectorlength(float *vec)
  {
  return(sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]));
  }

INLINE void normalizevector(float *result,float *vec)
  {
  float vectorlengthvalue;

  vectorlengthvalue=vectorlength(vec);
  if (vectorlengthvalue!=0.0f)
    {
    vectorlengthvalue=1.0f/vectorlengthvalue;
    result[0]=vec[0]*vectorlengthvalue;
    result[1]=vec[1]*vectorlengthvalue;
    result[2]=vec[2]*vectorlengthvalue;
    }
  else
    {
    result[0]=0.0f;
    result[1]=0.0f;
    result[2]=0.0f;
    }
  }

INLINE void copyvector(float *result,float *vec)
  {
  memcpy(result,vec,12);
  }

INLINE void negvector(float *result,float *vec)
  {
  result[0]=-vec[0];
  result[1]=-vec[1];
  result[2]=-vec[2];
  }

INLINE void zerovector(float *result)
  {
  memset(result,0,12);
  }

INLINE void addvectors(float *result,float *vec,float *vec2)
  {
  result[0]=vec[0]+vec2[0];
  result[1]=vec[1]+vec2[1];
  result[2]=vec[2]+vec2[2];
  }

INLINE void subtractvectors(float *result,float *vec,float *vec2)
  {
  result[0]=vec[0]-vec2[0];
  result[1]=vec[1]-vec2[1];
  result[2]=vec[2]-vec2[2];
  }

INLINE void scalevector(float *result,float *vec,float scale)
  {
  result[0]=vec[0]*scale;
  result[1]=vec[1]*scale;
  result[2]=vec[2]*scale;
  }

INLINE void scaleaddvectors(float *result,float *vec,float *vec2,float scale)
  {
  result[0]=vec[0]+vec2[0]*scale;
  result[1]=vec[1]+vec2[1]*scale;
  result[2]=vec[2]+vec2[2]*scale;
  }

INLINE void addvectors2(float *result,float *vec,float *vec2)
  {
  result[0]=vec[0]+vec2[0];
  result[1]=vec[1]+vec2[1];
  }

INLINE void subtractvectors2(float *result,float *vec,float *vec2)
  {
  result[0]=vec[0]-vec2[0];
  result[1]=vec[1]-vec2[1];
  }

INLINE void scalevector2(float *result,float *vec,float scale)
  {
  result[0]=vec[0]*scale;
  result[1]=vec[1]*scale;
  }

INLINE void scaleaddvectors2(float *result,float *vec,float *vec2,float scale)
  {
  result[0]=vec[0]+vec2[0]*scale;
  result[1]=vec[1]+vec2[1]*scale;
  }

void resetorientation(float orientation[3][3]);
void rotateorientation(float orientation[3][3],float *rotationvector,float rotationangle);

static float xaxis[3]={ 1.0f,0.0f,0.0f };
static float yaxis[3]={ 0.0f,1.0f,0.0f };
static float zaxis[3]={ 0.0f,0.0f,1.0f };

#endif
