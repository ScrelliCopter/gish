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

#include "animation.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SDL_opengl.h>
#include <sdl/platform.h>
#include <math/vector.h>
#include "random.h"
#include <video/texture.h>
#include "audio.h"
#include <physics/particle.h>
#include <physics/object.h>
#include <physfs.h>
#include "game.h"
#include "object.h"
#include "sprite.h"
#include <unistd.h>

int numofanimations;
struct ANIMATION animation[128];

void objectanimation(void)
  {
  int count,count2;
  int particlelist[2];
  float vec[3];
  float intersectpoint[3];
  float normal[3];
  float scale;

  for (count=0;count<numofobjects;count++)
    {
    if (object[count].type==1)
      {
      /*
      for (count2=0;count2<object[count].numofparticles;count2++)
      if ((rand()&8191)==0)
        createparticle(5,particle[object[count].particle[count2]].position,particle[object[count].particle[count2]].velocity,0.125f,-1,100);
      */

      if (object[count].frame==12)
        {
        object[count].framedelay+=0.125f;
        if (object[count].framedelay>=1.0f)
          {
          object[count].frame=0;
          object[count].framedelay=0.0f;
          }
        }

      if (object[count].numoforientations==0)
        {
        if (object[count].frame>=5 && object[count].frame<=7)
          object[count].frame=1;
        if (object[count].frame<4)
          {
          object[count].framedelay+=1.0f;
          if (object[count].framedelay>=4.0f)
            {
            object[count].frame++;
            object[count].framedelay=0.0f;
            }
          }
        }
      else
        {
        if (object[count].frame>0 && object[count].frame<=4)
          {
          object[count].framedelay+=1.0f;
          if (object[count].framedelay>=4.0f)
            {
            object[count].frame--;
            object[count].framedelay=0.0f;
            }
          }
        if (object[count].frame==0)
          if ((rnd()&255)==0)
            object[count].frame=5;

        if (object[count].frame>=5 && object[count].frame<=7)
          {
          object[count].framedelay+=1.0f;
          if (object[count].framedelay>=4.0f)
            {
            object[count].frame++;
            if (object[count].frame==8)
              object[count].frame=0;
            object[count].framedelay=0.0f;
            }
          }
        }
      if ((object[count].button&8)==8)
      if (object[count].frame<8)
        {
        object[count].frame=8;
        object[count].framedelay=0.0f;
        }

      if (object[count].frame>=8 && object[count].frame<=11)
        {
        object[count].framedelay+=1.0f;
        if (object[count].framedelay>=4.0f)
          {
          if ((object[count].button&8)==8)
            {
            if (object[count].frame<11)
              object[count].frame++;
            }
          else
            {
            if (object[count].frame>7)
              object[count].frame--;
            if (object[count].frame==7)
              object[count].frame=0;
            }
          object[count].framedelay=0.0f;
          }
        }
      }
    if (object[count].type==4)
      {
      if (object[count].animationtype==0 || object[count].animationtype==1)
      if ((object[count].button&1)==1)
        {
        object[count].animationtype=2;
        object[count].frame=0;
        object[count].framedelay=0.0f;
        }
      if (object[count].beasttype==12)
      if (object[count].animationtype==0)
      if ((object[count].button&2)==2)
        {
        object[count].animationtype=1;
        object[count].frame=0;
        object[count].framedelay=0.0f;
        }
      if (object[count].animationtype==0)
        {
        if (object[count].link!=-1)
          {
          object[object[count].link].texturenum=animation[object[count].animationnum+32].stand[0];
          if (fabs(object[0].position[0]-object[count].position[0])<1.5f)
          if (object[0].position[1]>object[count].position[1])
            object[object[count].link].texturenum=animation[object[count].animationnum+32].stand[1];
          }

        object[count].framedelay+=0.1f;
        if (object[count].framedelay>=1.0f)
          {
          object[count].framedelay=0.0f;
          object[count].frame++;
          if (object[count].frame>=animation[object[count].animationnum].stand[1])
            object[count].frame=0;
          }
        object[count].texturenum=animation[object[count].animationnum].stand[0]+object[count].frame;
        if (vectorlength(object[count].velocity)>=0.01f)
          {
          object[count].animationtype=1;
          object[count].frame=0;
          object[count].framedelay=0.0f;
          }
        }
      if (object[count].animationtype==1)
        {
        if (object[count].link!=-1)
          {
          object[object[count].link].texturenum=animation[object[count].animationnum+32].walk[0];
          if (fabs(object[0].position[0]-object[count].position[0])<1.5f)
          if (object[0].position[1]>object[count].position[1])
            object[object[count].link].texturenum=animation[object[count].animationnum+32].stand[1];
          }
        if (object[count].beasttype!=12)
          object[count].framedelay+=vectorlength(object[count].velocity)*6.0f;
        else
          object[count].framedelay+=0.2f;
        if (object[count].framedelay>=1.0f)
          {
          object[count].framedelay=0.0f;
          object[count].frame++;
          if (object[count].frame>=animation[object[count].animationnum].walk[1])
            {
            object[count].frame=0;
            if (object[count].beasttype==12)
              object[count].animationtype=0;
            }
          }
        object[count].texturenum=animation[object[count].animationnum].walk[0]+object[count].frame;
        if (object[count].beasttype!=12)
        if (vectorlength(object[count].velocity)<0.01f)
          {
          object[count].animationtype=0;
          object[count].frame=0;
          object[count].framedelay=0.0f;
          }
        if (object[count].beasttype==12)
        if (object[count].frame==4)
        if (object[count].framedelay==0.0f)
          {
          addvectors(vec,object[count].position,yaxis);
          createbeast(11,vec,1.0f,1.0f,1.0f,0.2f);
          object[numofobjects-1].timetolive=512;
          object[numofobjects-1].link=-1;
          for (count2=0;count2<4;count2++)
            {
            particle[object[numofobjects-1].particle[count2]].velocity[1]=0.08f;
            if (object[count].direction==0)
              particle[object[numofobjects-1].particle[count2]].velocity[0]=-0.04f;
            else
              particle[object[numofobjects-1].particle[count2]].velocity[0]=0.04f;
            }
          }
        }
      if (object[count].animationtype==2)
        {
        if (object[count].link!=-1)
          object[object[count].link].texturenum=animation[object[count].animationnum+32].attack[0];

        object[count].framedelay+=0.2f;
        if (object[count].framedelay>=1.0f)
          {
          object[count].framedelay=0.0f;
          object[count].frame++;
          if (object[count].frame>=animation[object[count].animationnum].attack[1])
            {
            object[count].animationtype=0;
            object[count].frame=0;
            }
          }
        object[count].texturenum=animation[object[count].animationnum].attack[0]+object[count].frame;

        if (object[count].frame==1)
        if (object[count].framedelay==0.0f)
          {
          if (object[count].beasttype<3 || object[count].beasttype==8 || object[count].beasttype==11 || object[count].beasttype==16)
            playsound(11,object[count].position,NULL,0.25f,0,0.7f,-1,0);
          else if (object[count].beasttype==7 || object[count].beasttype==13)
            playsound(12,object[count].position,NULL,0.5f,0,1.0f,-1,0);
          else
            playsound(13,object[count].position,NULL,0.5f,0,1.0f,-1,0);
          }

        if (object[count].frame==5)
          {
          //if (object[count].beasttype!=7 && object[count].beasttype!=13)
            {
            scale=1.25f;
            if (object[count].beasttype<3 || object[count].beasttype==8 || object[count].beasttype==11 || object[count].beasttype==16)
              scale=0.75f;
            if (object[count].beasttype==15)
              scale=1.5f;
            if (object[count].beasttype==7 || object[count].beasttype==13)
              scale=3.0f;
  
            if (object[count].direction==0)
              scaleaddvectors(vec,object[count].position,object[count].orientation[0],-scale);
            if (object[count].direction==1)
              scaleaddvectors(vec,object[count].position,object[count].orientation[0],scale);
            if (lineintersectobject(intersectpoint,normal,&scale,object[count].position,vec,0,particlelist))
              {
              scale=0.06f;
              if (object[count].beasttype<3)
                scale=0.03f;
              if (object[count].beasttype==15)
                scale=0.15f;
  
              if (object[count].direction==0)
                {
                scaleaddvectors(particle[particlelist[0]].velocity,particle[particlelist[0]].velocity,object[count].orientation[0],-scale);
                scaleaddvectors(particle[particlelist[1]].velocity,particle[particlelist[1]].velocity,object[count].orientation[0],-scale);
                }
              if (object[count].direction==1)
                {
                scaleaddvectors(particle[particlelist[0]].velocity,particle[particlelist[0]].velocity,object[count].orientation[0],scale);
                scaleaddvectors(particle[particlelist[1]].velocity,particle[particlelist[1]].velocity,object[count].orientation[0],scale);
                }
              if (object[count].beasttype<3 || object[count].beasttype==11)
                object[0].hitpoints-=20;
              else
                object[0].hitpoints-=40;
              if (object[count].beasttype==15)
                object[0].hitpoints-=40;
              }
            }
          if (object[count].beasttype==7 || object[count].beasttype==13)
          if (object[count].framedelay==0.0f)
            {
            if (object[count].beasttype==7)
              createbeast(2,object[count].position,1.0f,1.0f,1.0f,0.2f);
            else
              createbeast(8,object[count].position,1.0f,1.0f,1.0f,0.2f);
            object[numofobjects-1].timetolive=512;
            object[numofobjects-1].link=-1;
            for (count2=0;count2<4;count2++)
              {
              if (object[count].direction==0)
                particle[object[numofobjects-1].particle[count2]].velocity[0]=-0.08f;
              else
                particle[object[numofobjects-1].particle[count2]].velocity[0]=0.08f;
              }
            }
          }
        }
      if (object[count].timetolive>=0 && object[count].timetolive<45)
        {
        if (object[count].timetolive==44)
          {
          if (object[count].beasttype<3 || object[count].beasttype==8 || object[count].beasttype==11 || object[count].beasttype==16)
            {
            game.score[0]+=100*game.combo;
            createsprite(100*game.combo,object[count].position);
            game.combo++;
            game.combodelay+=100;
            if (game.combodelay>250)
              game.combodelay=250;
            playsound(7,object[count].position,NULL,1.0f,0,1.0f,-1,0);
            }
          else
            {
            game.score[0]+=200*game.combo;
            createsprite(200*game.combo,object[count].position);
            game.combo++;
            game.combodelay+=100;
            if (game.combodelay>250)
              game.combodelay=250;
            playsound(19,object[count].position,NULL,1.0f,0,1.0f,-1,0);
            }

          for (count2=0;count2<10;count2++)
            {
            vec[0]=(float)((rnd()&255)-127)/1270.0f;
            vec[1]=(float)((rnd()&255)-127)/1270.0f;
            vec[2]=0.0f;
            addvectors(vec,vec,object[count].velocity);

            createparticle(5,object[count].position,vec,0.25f,-1,100+(rnd()&63));
            particle[numofparticles-1].rendersize=0.125+(float)(rnd()&127)/1000.0f;
            particle[numofparticles-1].texturenum=367;
            if (object[count].beasttype==11 || object[count].beasttype==12)
              particle[numofparticles-1].texturenum=364;
            if (object[count].beasttype==0 || object[count].beasttype==3 || object[count].beasttype==4)
              particle[numofparticles-1].texturenum=365;
            }
          }

        if (object[count].link!=-1)
          {
          object[object[count].link].texturenum=animation[object[count].animationnum+32].die[0];
          if (object[object[count].link].timetolive>150)
            object[object[count].link].timetolive=150;
          }
        object[count].animationtype=3;
        object[count].frame=8-object[count].timetolive/5;
        object[count].framedelay=0.0f;
        object[count].texturenum=animation[object[count].animationnum].die[0]+object[count].frame;
        }
      }
    }
  if (game.levelnum==34)
  if (object[1].type==2)
  if ((rnd()&255)==0)
    {
    object[1].texturenum=64+(rnd()&3);
    }
  }

void loadanimationframe(const char* name, int index)
  {
  char filename[256];
  snprintf(filename,sizeof(filename),"animation/%s%02d.tga",name,index);
  loadtexturetga(numofanimations,filename,0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  numofanimations++;
  }

void loadanimation(const char* name,int *anipair,int numanims)
  {
    anipair[0]=numofanimations;
    anipair[1]=numanims;
    for (int i=1;i<=numanims;i++)
      {
      loadanimationframe(name,i);
      }
  }

int loadneeded(struct ANIMATION *anim)
  {
  if (anim->loaded==2)
    {
    anim->loaded=1;
    return 1;
    }
    return 0;
  }

void loadanimations(void)
  {
  int animidx;

  //bone nibbler
  animidx=0;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("bibsta",animation[animidx].stand,6);
    loadanimation("bibwlk",animation[animidx].walk,9);
    loadanimation("bibatk",animation[animidx].attack,9);
    loadanimation("nibdie",animation[animidx].die,9);
    }

  //cave nibbler
  animidx=1;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("nibsta",animation[animidx].stand,6);
    loadanimation("nibwlk",animation[animidx].walk,9);
    loadanimation("nibatk",animation[animidx].attack,9);
    loadanimation("nibdie",animation[animidx].die,9);
    }

  //hell nibbler
  animidx=2;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("ribsta",animation[animidx].stand,6);
    loadanimation("ribwlk",animation[animidx].walk,9);
    loadanimation("ribatk",animation[animidx].attack,9);
    loadanimation("ribdie",animation[animidx].die,9);
    }

  //mummy
  animidx=3;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("mumsta",animation[animidx].stand,6);
    loadanimation("mumwlk",animation[animidx].walk,9);
    loadanimation("mumatk",animation[animidx].attack,9);
    loadanimation("mumdie",animation[animidx].die,9);
  
    animidx+=32;
    animation[animidx].stand[0]=numofanimations;
    loadanimationframe("mumhed",1);

    animation[animidx].walk[0]=numofanimations;
    loadanimationframe("mumhed",2);

    animation[animidx].attack[0]=numofanimations;
    loadanimationframe("mumhed",3);

    animation[animidx].stand[1]=numofanimations;
    loadanimationframe("mumhed",5);

    animation[animidx].die[0]=numofanimations;
    loadanimationframe("mumhed",6);
    }

  //skeleton
  animidx=4;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("sklsta",animation[animidx].stand,6);
    loadanimation("sklwlk",animation[animidx].walk,9);
    loadanimation("sklatk",animation[animidx].attack,9);
    loadanimation("skldie",animation[animidx].die,9);

    animidx+=32;
    animation[animidx].stand[0]=numofanimations;
    loadanimationframe("sklhed",1);

    animation[animidx].walk[0]=numofanimations;
    loadanimationframe("sklhed",2);

    animation[animidx].attack[0]=numofanimations;
    loadanimationframe("sklhed",3);

    animation[animidx].stand[1]=numofanimations;
    loadanimationframe("sklhed",5);

    animation[animidx].die[0]=numofanimations;
    loadanimationframe("sklhed",6);
    }

  //zombie
  animidx=5;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("zomsta",animation[animidx].stand,6);
    loadanimation("zomwlk",animation[animidx].walk,9);
    loadanimation("zomatk",animation[animidx].attack,9);
    loadanimation("zomdie",animation[animidx].die,9);

    animidx+=32;
    animation[animidx].stand[0]=numofanimations;
    loadanimationframe("zomhed",1);

    animation[animidx].walk[0]=numofanimations;
    loadanimationframe("zomhed",2);

    animation[animidx].attack[0]=numofanimations;
    loadanimationframe("zomhed",3);

    animation[animidx].stand[1]=numofanimations;
    loadanimationframe("zomhed",5);

    animation[animidx].die[0]=numofanimations;
    loadanimationframe("zomhed",6);
    }

  //gimp
  animidx=6;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("gimsta",animation[animidx].stand,6);
    loadanimation("gimwlk",animation[animidx].walk,9);
    loadanimation("gimatk",animation[animidx].attack,9);
    loadanimation("gimdie",animation[animidx].die,9);

    animidx+=32;
    animation[animidx].stand[0]=numofanimations;
    loadanimationframe("gimhed",1);

    animation[animidx].walk[0]=numofanimations;
    loadanimationframe("gimhed",2);

    animation[animidx].attack[0]=numofanimations;
    loadanimationframe("gimhed",3);

    animation[animidx].stand[1]=numofanimations;
    loadanimationframe("gimhed",5);

    animation[animidx].die[0]=numofanimations;
    loadanimationframe("gimhed",6);
    }

  //vis sister
  animidx=7;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("vissta",animation[animidx].stand,6);
    loadanimation("viswlk",animation[animidx].walk,9);
    loadanimation("visatk",animation[animidx].attack,9);
    loadanimation("visdie",animation[animidx].die,9);
    }

  //angel nibbler
  animidx=8;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("aibsta",animation[animidx].stand,6);
    loadanimation("aibwlk",animation[animidx].walk,9);
    loadanimation("aibatk",animation[animidx].attack,9);
    loadanimation("aibdie",animation[animidx].die,9);
    }

  //alterboy (altar boy?)
  animidx=9;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("altsta",animation[animidx].stand,6);
    loadanimation("altwlk",animation[animidx].walk,9);
    loadanimation("altatk",animation[animidx].attack,9);
    loadanimation("altdie",animation[animidx].die,9);

    animidx+=32;
    animation[animidx].stand[0]=numofanimations;
    loadanimationframe("gimhed",1);

    animation[animidx].walk[0]=numofanimations;
    loadanimationframe("gimhed",2);

    animation[animidx].attack[0]=numofanimations;
    loadanimationframe("gimhed",3);

    animation[animidx].stand[1]=numofanimations;
    loadanimationframe("gimhed",5);

    animation[animidx].die[0]=numofanimations;
    loadanimationframe("gimhed",6);
    }

  //stitch
  animidx=10;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("frksta",animation[animidx].stand,6);
    loadanimation("frkwlk",animation[animidx].walk,9);
    loadanimation("frkatk",animation[animidx].attack,9);
    loadanimation("frkdie",animation[animidx].die,9);

    animidx+=32;
    animation[animidx].stand[0]=numofanimations;
    loadanimationframe("frkhed",1);

    animation[animidx].walk[0]=numofanimations;
    loadanimationframe("frkhed",2);

    animation[animidx].attack[0]=numofanimations;
    loadanimationframe("frkhed",3);

    animation[animidx].stand[1]=numofanimations;
    loadanimationframe("frkhed",5);

    animation[animidx].die[0]=numofanimations;
    loadanimationframe("frkhed",6);
    }

  //poobler
  animidx=11;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("pibsta",animation[animidx].stand,6);
    loadanimation("pibwlk",animation[animidx].walk,9);
    loadanimation("pibatk",animation[animidx].attack,9);
    loadanimation("pibdie",animation[animidx].die,9);
    }

  //honey bucket
  animidx=12;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("sepsta",animation[animidx].stand,6);
    loadanimation("sepspw",animation[animidx].walk,9);
    loadanimation("sepatk",animation[animidx].attack,8);
    loadanimation("sepdie",animation[animidx].die,9);
    }

  //sister vis
  animidx=13;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("sissta",animation[animidx].stand,6);
    loadanimation("siswlk",animation[animidx].walk,9);
    loadanimation("sisatk",animation[animidx].attack,9);
    loadanimation("sisdie",animation[animidx].die,9);
    }

  //paunchy chops
  animidx=14;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("ghosta",animation[animidx].stand,5);
    loadanimation("ghowlk",animation[animidx].walk,5);
    loadanimation("ghoatk",animation[animidx].attack,8);
    loadanimation("ghodie",animation[animidx].die,9);
    }

  //basinjin
  animidx=15;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("bassta",animation[animidx].stand,6);
    loadanimation("baswlk",animation[animidx].walk,9);
    loadanimation("basatk",animation[animidx].attack,9);
    loadanimation("basdie",animation[animidx].die,9);
    }

  //satan
  animidx=16;
  if (loadneeded(&animation[animidx]))
    {
    loadanimation("satsta",animation[animidx].stand,6);
    loadanimation("satwlk",animation[animidx].walk,9);
    loadanimation("satatk",animation[animidx].attack,9);
    loadanimation("ribdie",animation[animidx].die,9);
    }
  }
