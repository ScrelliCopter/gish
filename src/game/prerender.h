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

#ifndef __GISH_GAME_PRERENDER_H__
#define __GISH_GAME_PRERENDER_H__

void setuprenderobjects(void);
int setuprenderobjectlight(int objectnum);
int setuprenderropelight(int ropenum,float radius);

extern int numofobjectrenders;
struct OBJECTRENDER
  {
  int type;
  int texturenum;
  int objectnum;
  int lightflags;
  int numofverts;
  float vertex[64][3];
  float texcoord[64][2];
  float angle;
  int numoftris;
  int tri[64][3];
  int numofedges;
  float edgevertex[32][4][3];
  float edgetexcoord[32][4][2];
  float edgenormal[32][3];
  float depth;
  float alpha;
  };
extern struct OBJECTRENDER objectrender[512];

#endif
