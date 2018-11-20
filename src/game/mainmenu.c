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

#include "mainmenu.h"

#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <GL/gl.h>
#include <sdl/event.h>
#include <sdl/video.h>
#include <video/text.h>
#include <video/texture.h>
#include "lighting.h"
#include <input/keyboard.h>
#include <input/mouse.h>
#include <input/joystick.h>
#include <audio/audio.h>
#include "audio.h"
#include "music.h"
#include "config.h"
#include <math/vector.h>
#include "game.h"
#include "player.h"
#include "object.h"
#include "physics.h"
#include "animation.h"
#include "vsmode.h"
#include <menu/menu.h>
#include <config.h>
#include "editor.h"
#include "options.h"
#include "credits.h"
#include "socket.h"
#include "english.h"

void mainmenu(void)
  {
  int count,temp;
  int prevjoymenunum;
  int registered;
  char texfilename[256];
  unsigned int x,y;

  srand(time(NULL));
  count=(rand()%11)+1;

  snprintf(texfilename,sizeof(texfilename),"texture/loading%02d.tga",count);
  loadbackground(800,texfilename);

  glClearColor(0.0f,0.0f,0.0f,0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  setuptextdisplay();

  glColor4f(1.0f,1.0f,1.0f,1.0f);
  displaybackground(800);

  drawtext(TXT_LOADING,(320|TEXT_CENTER),448,16,1.0f,1.0f,1.0f,1.0f);

  SDL_GL_SwapWindow(sdlwindow);

  for (count=0;count<64;count++)
    {
    snprintf(texfilename,sizeof(texfilename),"texture/text%03d.tga",count);
    loadtexturetga(count+256,texfilename,0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
    }

  loadtexturetga(256+64,"texture/cage1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(256+65,"texture/cage2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(256+66,"texture/cage3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(256+67,"texture/cage4.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(256+68,"texture/x.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  setuplightingtextures();

  loadbackground(470,"texture/gishtitle.tga");

  loadtexturetga(462,"texture/complete.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(465,"texture/breatalk.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(468,"texture/lvlcomplete.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(469,"texture/gishy.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(512,"texture/gishtalk.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(513,"texture/honeytalk.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(514,"texture/paunchytalk.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(515,"texture/vistalk.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(516,"texture/bastalk.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(517,"texture/sistalk.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(518,"texture/heratalk.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(519,"texture/badending.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(520,"texture/goodending.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(521,"texture/sewerimg.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(522,"texture/caveimg.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(523,"texture/hellimg.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(524,"texture/egyptimg.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(525,"texture/churchimg.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(526,"texture/warpimg.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(527,"texture/satantalk.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(528,"texture/crypticsea.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(529,"texture/turbo.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(530,"texture/sumo.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(531,"texture/football.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(532,"texture/greed.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(533,"texture/pitfight.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(534,"texture/dragster.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(535,"texture/collect.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadbackground(540,"texture/level.tga");

  loadbackground(560,"texture/congrats.tga");
  loadbackground(580,"texture/blank.tga");

  loadbackground(600,"texture/vsmode.tga");
  loadbackground(620,"texture/storymode.tga");

  static const SDL_Rect borderrects[6]=
    {
    {0,0,256,256},
    {256,0,256,256},
    {512,0,256,256},
    {0,256,256,256},
    {256,256,256,256},
    {512,256,256,256}
    };
  loadtexturetgaparts(640,"texture/border.tga",borderrects,6);

  loadtexturetga(820,"texture/gishwin.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(821,"texture/graywin.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(822,"texture/fight.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(823,"texture/ready.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(700,"texture/0.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(701,"texture/1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(702,"texture/2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(703,"texture/3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(704,"texture/4.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(705,"texture/5.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(706,"texture/6.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(707,"texture/7.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(708,"texture/8.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(709,"texture/9.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(710,"texture/10.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(711,"texture/11.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(712,"texture/12.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(720,"texture/gishhud.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(721,"texture/greyhud.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(722,"texture/tarballs.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(723,"texture/key.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(360,"texture/rope.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(361,"texture/chain.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(362,"texture/piston1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(363,"texture/tar2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(364,"texture/poop.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(365,"texture/dust.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(366,"texture/bubble.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(367,"texture/blood.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(368,"texture/tar.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(369,"texture/amber.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(370,"texture/face.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(371,"texture/facem1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(372,"texture/facem2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(373,"texture/facem3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(374,"texture/facem4.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(375,"texture/faceb1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(376,"texture/faceb2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(377,"texture/faceb1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(378,"texture/faceh1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(379,"texture/faceh2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(380,"texture/faceh3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(381,"texture/faceh4.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(382,"texture/gishhurt.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(370+20,"texture/gface.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(371+20,"texture/gfacem1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(372+20,"texture/gfacem2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(373+20,"texture/gfacem3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(374+20,"texture/gfacem4.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(375+20,"texture/gfaceb1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(376+20,"texture/gfaceb2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(377+20,"texture/gfaceb1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(378+20,"texture/gfaceh01.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(379+20,"texture/gfaceh02.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(380+20,"texture/gfaceh03.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(381+20,"texture/gfaceh03.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(382+20,"texture/grayhurt.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(370+40,"texture/bface.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(371+40,"texture/bfacem1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(372+40,"texture/bfacem2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(373+40,"texture/bfacem3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(374+40,"texture/bfacem4.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(375+40,"texture/bfaceb1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(376+40,"texture/bfaceb2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(377+40,"texture/bfaceb1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(378+40,"texture/bfaceh1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(379+40,"texture/bfaceh2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(380+40,"texture/bfaceh3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(381+40,"texture/bfaceh3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(382+40,"texture/bhurt.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(383+40,"texture/btar.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  loadtexturetga(370+60,"texture/rface.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(371+60,"texture/rfacem1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(372+60,"texture/rfacem2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(373+60,"texture/rfacem3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(374+60,"texture/rfacem4.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(375+60,"texture/rfaceb1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(376+60,"texture/rfaceb2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(377+60,"texture/rfaceb1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(378+60,"texture/rfaceh1.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(379+60,"texture/rfaceh2.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(380+60,"texture/rfaceh3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(381+60,"texture/rfaceh3.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(382+60,"texture/rhurt.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
  loadtexturetga(383+60,"texture/rtar.tga",0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

  numofanimations=1024;

  //loadanimations();

  if (config.sound)
    loadoggs();

  setupphysics();

  setupobjecttypes();

  editor.mode=1;

  game.songnum=-1;
  game.currentsongnum=-1;
  resetorientation(view.orientation);
  soundsimulation(view.position,view.orientation);

  introscreen();

  resetmenuitems();

  joymenunum=1;
  game.turbomode=1;

  while (!menuitem[0].active && !windowinfo.shutdown)
    {
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    numofmenuitems=0;
		temp=(sizeof(TXT_EXIT)+sizeof(TXT_CREDITS)+sizeof(TXT_OPTIONS)+sizeof(TXT_VERSUS)+sizeof(TXT_START)-1)*8;

    createmenuitem(TXT_EXIT,(320+temp)|TEXT_END,380,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_ESC);
    createmenuitem(TXT_START,320-temp,380,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_S);
		temp-=(sizeof(TXT_START))*16;
    createmenuitem(TXT_VERSUS,320-temp,380,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_V);
		temp-=(sizeof(TXT_VERSUS))*16;
    createmenuitem(TXT_OPTIONS,320-temp,380,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_O);
		temp-=(sizeof(TXT_OPTIONS))*16;
    createmenuitem(TXT_CREDITS,320-temp,380,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_C);
    createmenuitem("Cryptic Sea",0,0,1,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_IMAGE,528);
    setmenuitem(MO_RESIZE,16,0,96,48);

    checksystemmessages();
    checkkeyboard();
    checkmouse();
    checkjoystick();
    checkmenuitems();

    if (keyboard[SCAN_T] && !prevkeyboard[SCAN_T])
      game.turbomode^=1;

    updateogg();
    checkmusic();
    game.songnum=8;
    soundsimulation(view.position,view.orientation);

    setuptextdisplay();

    glColor4f(1.0f,1.0f,1.0f,1.0f);
    displaybackground(470);

    if (game.turbomode)
      drawtext(TXT_TURBO_EDITION,(640|TEXT_END),470,10,1.0f,1.0f,1.0f,1.0f);
    drawtext(VERSIONTEXT,0,470,10,1.0f,1.0f,1.0f,1.0f);

    drawtext(TXT_COPYRIGHT,(320|TEXT_CENTER),470,10,0.75f,0.75f,0.75f,1.0f);

    drawmenuitems();

    drawmousecursor(768+font.cursornum,mouse.x,mouse.y,16,1.0f,1.0f,1.0f,1.0f);

    SDL_GL_SwapWindow(sdlwindow);

    if (menuitem[5].active)
      {
      launchwebpage("www.crypticsea.com");
      menuitem[5].active=0;
      }
    if (menuitem[4].active)
      {
      //game.songnum=-1;
      //checkmusic();

      prevjoymenunum=4;
      creditsmenu();
      joymenunum=prevjoymenunum;
      }
    if (menuitem[3].active)
      {
      game.songnum=-1;
      checkmusic();

      prevjoymenunum=3;
      optionsmenu();
      joymenunum=prevjoymenunum;
      }
    if (menuitem[2].active)
      {
      //game.songnum=-1;
      //checkmusic();

      prevjoymenunum=2;
      versusmenu();
      joymenunum=prevjoymenunum;
      }
    if (menuitem[1].active)
      {
      //game.songnum=-1;
      //checkmusic();

      prevjoymenunum=1;
      playermenu();
      joymenunum=prevjoymenunum;
      }
    }

  game.songnum=-1;
  checkmusic();
  }

void loadbackground(int texturenum,char *filename)
  {
  static const SDL_Rect bgrect[12]=
    {
    {0,0,256,256},
    {256,0,256,256},
    {512,0,256,256},
    {768,0,256,256},
    {0,256,256,256},
    {256,256,256,256},
    {512,256,256,256},
    {768,256,256,256},
    {0,512,256,256},
    {256,512,256,256},
    {512,512,256,256},
    {768,512,256,256}
    };
  loadtexturetgaparts(texturenum,filename,bgrect,12);
  }

void displaybackground(int texturenum)
  {
  drawbackground(texturenum,0,0,256,256,800,600);
  drawbackground(texturenum+1,256,0,256,256,800,600);
  drawbackground(texturenum+2,512,0,256,256,800,600);
  drawbackground(texturenum+3,768,0,256,256,800,600);
  drawbackground(texturenum+4,0,256,256,256,800,600);
  drawbackground(texturenum+5,256,256,256,256,800,600);
  drawbackground(texturenum+6,512,256,256,256,800,600);
  drawbackground(texturenum+7,768,256,256,256,800,600);
  drawbackground(texturenum+8,0,512,256,256,800,600);
  drawbackground(texturenum+9,256,512,256,256,800,600);
  drawbackground(texturenum+10,512,512,256,256,800,600);
  drawbackground(texturenum+11,768,512,256,256,800,600);
  }

void versusmenu(void)
  {
  int count;
  int unlocked;

  unlocked=0;
  for (count=0;count<6;count++)
  if (player[count].unlock[0])
    unlocked=1;

  resetmenuitems();

  joymenunum=1;

  while (!menuitem[0].active && !windowinfo.shutdown)
    {
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    numofmenuitems=0;
    createmenuitem(TXT_BACK,0,0,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_ESC);

    count=64;
    createmenuitem(TXT_SUMO,(144|TEXT_CENTER),304,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_S);
    createmenuitem(TXT_FOOTBALL,(320|TEXT_CENTER),304,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_F);
    createmenuitem(TXT_GREED,(512|TEXT_CENTER),304,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_G);
    createmenuitem(TXT_PITFIGHT,(152|TEXT_CENTER),444,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_P);
    createmenuitem(TXT_DRAGSTER,(320|TEXT_CENTER),444,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_D);
    if (unlocked)
      {
      createmenuitem(TXT_COLLECTION,(512|TEXT_CENTER),444,16,1.0f,1.0f,1.0f,1.0f);
      setmenuitem(MO_HOTKEY,SCAN_C);
      }

    checksystemmessages();
    checkkeyboard();
    checkmouse();
    checkjoystick();
    checkmenuitems();

    updateogg();
    checkmusic();
    game.songnum=8;
    soundsimulation(view.position,view.orientation);

    setuptextdisplay();

    glColor4f(1.0f,1.0f,1.0f,1.0f);
    displaybackground(600);

    if (!unlocked)
      {
      drawbackground(256+68,448,328,96,96,640,480);
      drawtext("?????",(512|TEXT_CENTER),444,16,1.0f,1.0f,1.0f,1.0f);
      }

    drawmenuitems();


    drawmousecursor(768+font.cursornum,mouse.x,mouse.y,16,1.0f,1.0f,1.0f,1.0f);

    SDL_GL_SwapWindow(sdlwindow);

    for (count=1;count<=7;count++)
    if (menuitem[count].active)
      versusmodemenu(count-1);
    }

  resetmenuitems();
  }

void storyscreen(void)
  {
  int count;
  int simtimer;
  int simcount;
  int loaddelay;
  int screennum;

  loaddelay=0;
  screennum=0;

  loadbackground(824,"texture/story1.tga");

  simtimer=SDL_GetTicks();

  resetmenuitems();

  joymenunum=1;

  while (!menuitem[0].active && screennum<8 && !windowinfo.shutdown)
    {
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    numofmenuitems=0;
    createmenuitem(TXT_BACK,0,0,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_ESC);
    createmenuitem(TXT_NEXT,(640|TEXT_END),0,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_N);

    checksystemmessages();
    checkkeyboard();
    checkmouse();
    checkjoystick();
    checkmenuitems();

    updateogg();
    checkmusic();
    game.songnum=8;
    soundsimulation(view.position,view.orientation);

    setuptextdisplay();

    glColor4f(1.0f,1.0f,1.0f,1.0f);
    displaybackground(824);

    drawmenuitems();

    drawmousecursor(768+font.cursornum,mouse.x,mouse.y,16,1.0f,1.0f,1.0f,1.0f);

    SDL_GL_SwapWindow(sdlwindow);

    simcount=0;
    while (SDL_GetTicks()-simtimer>20 && simcount<5)
      {
      simcount++;
      count=SDL_GetTicks()-simtimer-20;
      simtimer=SDL_GetTicks()-count;

      loaddelay++;
      if (loaddelay>=400)
        {
        screennum++;
        if (screennum==1)
          loadbackground(824,"texture/story2.tga");
        if (screennum==2)
          loadbackground(824,"texture/story3.tga");
        if (screennum==3)
          loadbackground(824,"texture/story4.tga");
        if (screennum==4)
          loadbackground(824,"texture/story5.tga");
        if (screennum==5)
          loadbackground(824,"texture/story6.tga");
        if (screennum==6)
          loadbackground(824,"texture/story7.tga");
        if (screennum==7)
          loadbackground(824,"texture/story8.tga");
        loaddelay=0;
        }
      }
    if (menuitem[1].active)
      {
      screennum++;
      if (screennum==1)
        loadbackground(824,"texture/story2.tga");
      if (screennum==2)
        loadbackground(824,"texture/story3.tga");
      if (screennum==3)
        loadbackground(824,"texture/story4.tga");
      if (screennum==4)
        loadbackground(824,"texture/story5.tga");
      if (screennum==5)
        loadbackground(824,"texture/story6.tga");
      if (screennum==6)
        loadbackground(824,"texture/story7.tga");
      if (screennum==7)
        loadbackground(824,"texture/story8.tga");
      loaddelay=0;
      menuitem[1].active=0;
      }
    }

  resetmenuitems();
  }

void introscreen(void)
  {
  int count,count2;
  int simtimer;
  int simcount;
  int loaddelay;
  float alpha;
  int secretsequence;
  float vec[3];

  srand(time(NULL));

  loaddelay=0;

  secretsequence=0;

  simtimer=SDL_GetTicks();

  resetmenuitems();

  while (!menuitem[0].active && loaddelay<200 && !windowinfo.shutdown)
    {
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    numofmenuitems=0;
    createmenuitem(" ",0,0,640,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_ESC);

    checksystemmessages();
    checkkeyboard();
    checkmouse();
    checkjoystick();
    checkmenuitems();

    if (secretsequence==0)
      count2=3;
    if (secretsequence==1)
      count2=3;
    if (secretsequence==2)
      count2=2;
    if (secretsequence==3)
      count2=2;
    if (secretsequence==4)
      count2=0;
    if (secretsequence==5)
      count2=1;
    if (secretsequence==6)
      count2=0;
    if (secretsequence==7)
      count2=1;
    if (secretsequence==8)
      count2=4;
    if (secretsequence==9)
      count2=5;
    if (secretsequence==10)
      {
      if (game.turbomode==0)
        {
        vec[0]=0.0f;
        vec[1]=0.0f;
        vec[2]=0.0f;
        playsound(10,vec,NULL,0.4f,0,1.0f,-1,0);
        }
      game.turbomode=1;
      }

    for (count=0;count<8;count++)
      {
      if (keyboard[control[0].key[count]] && !prevkeyboard[control[0].key[count]])
        {
        if (count==count2)
          secretsequence++;
        else
          secretsequence=0;
        }

      if (control[0].joysticknum!=-1)
        {
        if (count==0)
        if (joystick[control[0].joysticknum].axis[0]<=-0.5f && prevjoystick[control[0].joysticknum].axis[0]>-0.5f)
          {
          if (count==count2)
            secretsequence++;
          else
            secretsequence=0;
          }
        if (count==1)
        if (joystick[control[0].joysticknum].axis[0]>=0.5f && prevjoystick[control[0].joysticknum].axis[0]<0.5f)
          {
          if (count==count2)
            secretsequence++;
          else
            secretsequence=0;
          }
        if (count==2)
        if (joystick[control[0].joysticknum].axis[1]<=-0.5f && prevjoystick[control[0].joysticknum].axis[1]>-0.5f)
          {
          if (count==count2)
            secretsequence++;
          else
            secretsequence=0;
          }
        if (count==3)
        if (joystick[control[0].joysticknum].axis[1]>=0.5f && prevjoystick[control[0].joysticknum].axis[1]<0.5f)
          {
          if (count==count2)
            secretsequence++;
          else
            secretsequence=0;
          }
        if (control[0].button[count]!=-1)
        if (joystick[control[0].joysticknum].button[control[0].button[count]] && !prevjoystick[control[0].joysticknum].button[control[0].button[count]])
          {
          if (count==count2)
            secretsequence++;
          else
            secretsequence=0;
          }
        }
      }

    updateogg();
    checkmusic();
    game.songnum=8;
    soundsimulation(view.position,view.orientation);

    setuptextdisplay();

    if (loaddelay<100)
      {
      alpha=(float)(100-loaddelay)/100.0f;
      glColor4f(1.0f,1.0f,1.0f,alpha);
      displaybackground(800);
      }
    else
      {
      alpha=(float)(loaddelay-100)/100.0f;
      glColor4f(1.0f,1.0f,1.0f,alpha);
      displaybackground(470);

      drawtext(TXT_EXIT,(320+72+128),380,16,0.5f,0.5f,0.5f,alpha);
      drawtext(TXT_START,(320-72-112)|TEXT_END,380,16,0.5f,0.5f,0.5f,alpha);
      drawtext(TXT_VERSUS,(320-72)|TEXT_END,380,16,0.5f,0.5f,0.5f,alpha);
      drawtext(TXT_OPTIONS,(320)|TEXT_CENTER,380,16,0.5f,0.5f,0.5f,alpha);
      drawtext(TXT_CREDITS,(320+72),380,16,0.5f,0.5f,0.5f,alpha);

      if (game.turbomode)
        drawtext(TXT_TURBO_EDITION,(640|TEXT_END),470,10,1.0f,1.0f,1.0f,1.0f);
      drawtext(VERSIONTEXT,0,470,10,1.0f,1.0f,1.0f,1.0f);

      drawtext(TXT_COPYRIGHT,(320|TEXT_CENTER),470,10,0.75f,0.75f,0.75f,alpha);
      }
    drawmenuitems();

    drawmousecursor(768+font.cursornum,mouse.x,mouse.y,16,1.0f,1.0f,1.0f,1.0f);

    SDL_GL_SwapWindow(sdlwindow);

    simcount=0;
    while (SDL_GetTicks()-simtimer>20 && simcount<5)
      {
      simcount++;
      count=SDL_GetTicks()-simtimer-20;
      simtimer=SDL_GetTicks()-count;

      loaddelay++;
      }
    }

  resetmenuitems();
  }
