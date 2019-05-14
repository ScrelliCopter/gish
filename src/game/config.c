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

#include "config.h"

#include <config.h>
#include <stdio.h>
#include <errno.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <physfs.h>
#include <sdl/event.h>
#include <sdl/video.h>
#include <video/text.h>
#include <input/keyboard.h>
#include <input/mouse.h>
#include "options.h"
#include "socket.h"
#include <menu/menu.h>

struct CONFIG config;
static const char *filename="config.txt";

void loadconfigdefaults(void)
  {
  int count;

  config.displayid=0;
  config.resolutionx=800;
  config.resolutiony=600;
  config.bitsperpixel=32;
  config.refreshrate=60;
  config.depthbits=24;
  config.stencilbits=8;
  config.fullscreen=0;
  config.sound=1;
  config.music=1;
  config.joystick=1;

  option.sound=1;
  option.music=1;
  option.soundvolume=1.0f;
  option.musicvolume=1.0f;

  control[0].key[0]=SCAN_LEFT;
  control[0].key[1]=SCAN_RIGHT;
  control[0].key[2]=SCAN_DOWN;
  control[0].key[3]=SCAN_UP;
  control[0].key[4]=SCAN_A;
  control[0].key[5]=SCAN_SPACE;
  control[0].key[6]=SCAN_S;
  control[0].key[7]=SCAN_D;
  control[0].joysticknum=0;
  control[0].axis[0]=0;
  control[0].axis[1]=1;
  for (count=0;count<4;count++)
    control[0].button[count]=-1;
  for (count=0;count<4;count++)
    control[0].button[count+4]=count;
  control[0].button[8]=5;

  control[1].joysticknum=-1;
  control[1].axis[0]=0;
  control[1].axis[1]=1;
  for (count=0;count<4;count++)
    control[1].button[count]=-1;
  for (count=0;count<4;count++)
    control[1].button[count+4]=count;

  control[2].joysticknum=-1;
  control[2].axis[0]=0;
  control[2].axis[1]=1;
  for (count=0;count<4;count++)
    control[2].button[count]=-1;
  for (count=0;count<4;count++)
    control[2].button[count+4]=count;

  control[3].joysticknum=-1;
  control[3].axis[0]=0;
  control[3].axis[1]=1;
  for (count=0;count<4;count++)
    control[3].button[count]=-1;
  for (count=0;count<4;count++)
    control[3].button[count+4]=count;
  }

int strstartswith(const char *a,const char *b)
  {
  return !memcmp(a,b,strlen(b));
  }

int parseint(const char *str,int *out)
  {
  char* cptr;
  long res;

  res=strtol(str,&cptr,0);
  if (cptr==NULL || cptr==str)
    return 0;
  if (errno==ERANGE)
    return 0;

  (*out)=(int)res;
  return 1;
  }

void parseline(const char *line)
  {
  char key[32];
  char* cptr;
  int tmp;

  if (strstartswith(line,"#"))
    return;

  cptr=strchr(line,'=');
  if (cptr==NULL || cptr==line)
    return;
  cptr++;

  if (strstartswith(line,"display="))
    parseint(cptr,&config.displayid);
  else if (strstartswith(line,"screenwidth="))
    parseint(cptr,&config.resolutionx);
  else if (strstartswith(line,"screenheight="))
    parseint(cptr,&config.resolutiony);
  else if (strstartswith(line,"bitsperpixel="))
    parseint(cptr,&config.bitsperpixel);
  else if (strstartswith(line,"refreshrate="))
    parseint(cptr,&config.refreshrate);
  else if (strstartswith(line,"depthbits="))
    parseint(cptr,&config.depthbits);
  else if (strstartswith(line,"stencilbits="))
    parseint(cptr,&config.stencilbits);
  else if (strstartswith(line,"fullscreen="))
    parseint(cptr,&config.fullscreen);
  else if (strstartswith(line,"sound="))
    parseint(cptr,&config.sound);
  else if (strstartswith(line,"music="))
    parseint(cptr,&config.music);
  else if (strstartswith(line,"joystick="))
    parseint(cptr,&config.joystick);

  else if (strstartswith(line,"soundon="))
    parseint(cptr,&option.sound);
  else if (strstartswith(line,"musicon="))
    parseint(cptr,&option.music);
  else if (strstartswith(line,"soundvolume="))
    {
    if (parseint(cptr,&tmp))
      option.soundvolume=(float)tmp/100.f;
    }
  else if (strstartswith(line,"musicvolume="))
    {
    if (parseint(cptr,&tmp))
      option.musicvolume=(float)tmp/100.f;
    }
  else for (int i=0;i<4;i++)
    {
    for (int j=0;j<16;j++)
      {
      snprintf(key,sizeof(key),"player%dkey%d=",i+1,j+1);
      if (strstartswith(line,key))
        {
        parseint(cptr,&control[i].key[j]);
        return;
        }
      }

    snprintf(key,sizeof(key),"player%djoysticknum=",i+1);
    if (strstartswith(line,key))
      {
      parseint(cptr,&control[i].joysticknum);
      return;
      }

    for (int j=0;j<4;j++)
      {
      snprintf(key,sizeof(key),"player%daxis%d=",i+1,j+1);
      if (strstartswith(line,key))
        {
        parseint(cptr,&control[i].axis[j]);
        return;
        }
      }
    for (int j=0;j<16;j++)
      {
      snprintf(key,sizeof(key),"player%dbutton%d=",i+1,j+1);
      if (strstartswith(line,key))
        {
        parseint(cptr,&control[i].button[j]);
        return;
        }
      }
    }
  }

void loadconfig(void)
  {
  char tempstr[256];
  PHYSFS_file *fp;

  loadconfigdefaults();
  if (!PHYSFS_exists(filename))
    {
#ifdef DEBUG
    fprintf(stdout,"loadconfig(): file \"%s\" not found, but should be generated soon\n",filename);
#endif
    goto copywininfo;
    }

  if ((fp=PHYSFS_openRead(filename))==NULL)
    {
    fprintf(stderr,"PHYSFS_openRead(): %s\n",PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    fprintf(stderr,"loadconfig(): failed to load \"%s\"\n",filename);
    goto copywininfo;
    }

  int i=0;
  while (!PHYSFS_eof(fp))
    {
    char c;
    PHYSFS_readBytes(fp,&c,1);
    if (c=='\n' || i>=sizeof(tempstr))
      {
      tempstr[i]='\0';
      parseline(tempstr);
      i=0;
      }
    else if (c!='\0')
      {
      tempstr[i++]=c;
      }
    }


  PHYSFS_close(fp);

copywininfo:
  windowinfo.displayid=config.displayid;
  windowinfo.resolutionx=config.resolutionx;
  windowinfo.resolutiony=config.resolutiony;
  windowinfo.bitsperpixel=config.bitsperpixel;
  windowinfo.refreshrate=config.refreshrate;
  windowinfo.depthbits=config.depthbits;
  windowinfo.stencilbits=config.stencilbits;
  windowinfo.fullscreen=config.fullscreen;
  }

void optionwriteint(PHYSFS_file *fp,int *ptr,char *str)
  {
  char tmpline[256];
  int res=snprintf(tmpline,sizeof(tmpline),"%s%d\r\n",str,*ptr);
  if (res>0)
    {
    PHYSFS_writeBytes(fp,tmpline,(size_t)res);
    }
  }

void saveconfig(void)
  {
  int tmp;
  char tempstr[256];
  PHYSFS_file *fp;

  config.displayid=windowinfo.displayid;
  config.resolutionx=windowinfo.resolutionx;
  config.resolutiony=windowinfo.resolutiony;
  config.bitsperpixel=windowinfo.bitsperpixel;
  config.refreshrate=windowinfo.refreshrate;
  config.depthbits=windowinfo.depthbits;
  config.stencilbits=windowinfo.stencilbits;
  config.fullscreen=windowinfo.fullscreen;

  if ((fp=PHYSFS_openWrite(filename))==NULL)
    {
    fprintf(stderr,"PHYSFS_openWrite(): %s\n",PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    fprintf(stderr,"saveconfig(): failed to save \"%s\"\n",filename);
    return;
    }

  optionwriteint(fp,&config.displayid,"display=");
  optionwriteint(fp,&config.resolutionx,"screenwidth=");
  optionwriteint(fp,&config.resolutiony,"screenheight=");
  optionwriteint(fp,&config.bitsperpixel,"bitsperpixel=");
  optionwriteint(fp,&config.refreshrate,"refreshrate=");
  optionwriteint(fp,&config.depthbits,"depthbits=");
  optionwriteint(fp,&config.stencilbits,"stencilbits=");
  optionwriteint(fp,&config.fullscreen,"fullscreen=");
  optionwriteint(fp,&config.sound,"sound=");
  optionwriteint(fp,&config.music,"music=");
  optionwriteint(fp,&config.joystick,"joystick=");

  optionwriteint(fp,&option.sound,"soundon=");
  optionwriteint(fp,&option.music,"musicon=");
  tmp=(int)(option.soundvolume*100.0f);
  optionwriteint(fp,&tmp,"soundvolume=");
  tmp=(int)(option.musicvolume*100.0f);
  optionwriteint(fp,&tmp,"musicvolume=");

  for (int i=0;i<4;i++)
    {
    for (int j=0;j<16;j++)
      {
      snprintf(tempstr,sizeof(tempstr),"player%dkey%d=",i+1,j+1);
      optionwriteint(fp,&control[i].key[j],tempstr);
      }

    snprintf(tempstr,sizeof(tempstr),"player%djoysticknum=",i+1);
    optionwriteint(fp,&control[i].joysticknum,tempstr);

    for (int j=0;j<4;j++)
      {
      snprintf(tempstr,sizeof(tempstr),"player%daxis%d=",i+1,j+1);
      optionwriteint(fp,&control[i].axis[j],tempstr);
      }
    for (int j=0;j<16;j++)
      {
      snprintf(tempstr,sizeof(tempstr),"player%dbutton%d=",i+1,j+1);
      optionwriteint(fp,&control[i].button[j],tempstr);
      }
    }

  PHYSFS_close(fp);
  }

void notsupportedmenu(void)
  {
  int count;
  char *glvendor;
  char *glrenderer;
  char *glversion;
  char *ext;
  FILE *fp;

  glvendor=(char *) glGetString(GL_VENDOR);
  glrenderer=(char *) glGetString(GL_RENDERER);
  glversion=(char *) glGetString(GL_VERSION);
  ext=(char *) glGetString(GL_EXTENSIONS);

  resetmenuitems();

  while (!menuitem[0].active && !windowinfo.shutdown)
    {
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    numofmenuitems=0;
    createmenuitem("Exit",8,8,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_ESC);
    count=352;
    createmenuitem("NVIDIA Drivers",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("ATI Drivers",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("Intel Drivers",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;

    checksystemmessages();
    checkkeyboard();
    checkmouse();
    checkmenuitems();

    setuptextdisplay();

    count=224;
    drawtext("Multitexture required",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    count+=16;
    drawtext("Make sure you have the",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    drawtext("latest video card drivers",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    drawtext("for your computer",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);

    count=432;
    drawtext("GL_VERSION: /s",8,count,12,1.0f,1.0f,1.0f,1.0f,glversion);
    count+=12;
    drawtext("GL_VENDOR: /s",8,count,12,1.0f,1.0f,1.0f,1.0f,glvendor);
    count+=12;
    drawtext("GL_RENDERER: /s",8,count,12,1.0f,1.0f,1.0f,1.0f,glrenderer);

    drawmenuitems();

    drawmousecursor(768+font.cursornum,mouse.x,mouse.y,16,1.0f,1.0f,1.0f,1.0f);

    SDL_GL_SwapWindow(sdlwindow);

    if (menuitem[1].active)
      {
      launchwebpage("www.nvidia.com/content/drivers/drivers.asp");
      menuitem[1].active=0;
      }
    if (menuitem[2].active)
      {
      launchwebpage("ati.amd.com/support/driver.html");
      menuitem[2].active=0;
      }
    if (menuitem[3].active)
      {
      launchwebpage("downloadcenter.intel.com");
      menuitem[3].active=0;
      }
    }

  resetmenuitems();

  if ((fp=fopen("glreport.txt","wb"))==NULL)
    return;

  fprintf(fp,"%s\r\n",glversion);
  fprintf(fp,"%s\r\n",glvendor);
  fprintf(fp,"%s\r\n",glrenderer);
  fprintf(fp,"%s\r\n",ext);
  }
