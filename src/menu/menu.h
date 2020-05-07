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

#ifndef __GISH_MENU_MENU_H__
#define __GISH_MENU_MENU_H__

#define MAXMENUITEMS   256
#define MO_HOTKEY       1
#define MO_HIGHLIGHT    2
#define MO_INTINPUT     3
#define MO_UINTINPUT    4
#define MO_FLOATINPUT   5
#define MO_STRINGINPUT  6
#define MO_FUNCTION     7
#define MO_TOGGLE       8
#define MO_SET          9
#define MO_REPEAT      10
#define MO_IMAGE       11
#define MO_RESIZE      12
#define MO_CURSOR      13
#define MO_BACKGROUND  14

void checkmenuitems(void);
void drawmenuitems(void);
void createmenuitem(char *label,int x,int y,int textsize,float r,float g,float b,float a);
void setmenuitem(int option,...);
void createmenuitemempty(void);
void resetmenuitems(void);
void setupmenuitems(void);
void menutextbox(void);
void menutoggle(void);
void menuset(void);
int getinputletter(int repeat,int inputflags);
void addmenuinputchar(int charinput);
int deleteselectedtext(void);

extern int numofmenuitems;
#define MENUITEM_LABEL_LEN 64
struct MENUITEM
  {
  char label[MENUITEM_LABEL_LEN];
	int x;
	int y;
	int textsize;
	float r;
	float g;
	float b;
	int type;
	int sizex;
	int sizey;
	int hotkey;
  int repeat;
	int highlight;
	int prevhighlight;
  int prevactive;
	int active;
  int value;
  int inputtype;
	int texturenum;
  int cursornum;
  int background;
  void *inputpointer;
  void (*function)();
  };
extern struct MENUITEM menuitem[MAXMENUITEMS];

extern int menuinputkeyboard;
extern int menuinputselectpos;
extern int menuinputcursorpos;
extern int menuinputinsert;

#define MENUINPUT_LEN 256
extern char menuinput[MENUINPUT_LEN];
extern char menuinputtemp[MENUINPUT_LEN];

extern int joymenunum;
extern int joystickmenu;

extern char keyboardlabel[323][16];

#endif
