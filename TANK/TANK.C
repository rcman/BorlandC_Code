/*****************************************************************************
 *  Super Tank (C) Copyright 1992 by Kevin P. Dahlhausen   ap096@po.cwru.edu
 *****************************************************************************/


/* TANK.C contains external data declarations, initialization, and main() */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include <bios.h>

#include "stk.h"       /* keyboard hanlder */
#include "tankdefs.h"
#include "types.h"
#include "extern.h"
#include "tankpics.h"
#include "joy.h"

/*************************************************************************/
/* Global data declarations:
 * -EXTERN.H should reflect any changes here
 * -the function of each variable is documented in EXTERN.H
 */
int	HitX,HitY;
int	NumTanks=DEFNUMTANKS;
int	TankSpeed=DEFTANKSPEED;
int	ShotSpeed=DEFSHOTSPEED;
int	ShotRange=DEFSHOTRANGE;
int   TreesOn=0;
int   GuidedShots=0;
int	newscreen=1;
int	TankInitialX[]={301, 8, 154, 154};   /* initial tank x coordinates */
int	TankInitialY[]={100, 100, 174, 15}; /* initial tank y coordinates */
int	tankColor[]={T0COLOR, T1COLOR, T2COLOR, T3COLOR};
char far *vbuf = (char far *) 0xA0000000;
unsigned char field[FIELDX][FIELDY];
shotrec shotdata[MAXNUMTANKS];
tankRec tank[MAXNUMTANKS];
treeRec tree[NUMTREES];

joy_stick joystick;
int JoyStickNumber=-1;
int StickOnTank=-1;

/*************************************************************************/
/* local decalarations
 */

/* the following are the scan codes for the default keys by tank number */
char UpKeys[]   ={0x48, 0x11, 0x19, 0x15};
char DownKeys[] ={0x50, 0x2c, 0x34, 0x30};
char LeftKeys[] ={0x4b, 0x1e, 0x26, 0x22};
char RightKeys[]={0x4d, 0x20, 0x28, 0x24};
char FireKeys[] ={0x47, 0x10, 0x18, 0x14};


void init()
/* requires: none
 * ensures:  game variables initialized
 */
{
	int i,i1;

	/* initialize the tank records */
	for (i=0;i<MAXNUMTANKS;i++) {
		/* tank pictures */
	   for(i1=0;i1<TBMLEN;i1++) tank[i].pic[UP][i1]=TankUpBM[i1]*tankColor[i];
	   for(i1=0;i1<TBMLEN;i1++) tank[i].pic[DOWN][i1]=TankDownBM[i1]*tankColor[i];
		for(i1=0;i1<TBMLEN;i1++) tank[i].pic[LEFT][i1]=TankLeftBM[i1]*tankColor[i];
		for(i1=0;i1<TBMLEN;i1++) tank[i].pic[RIGHT][i1]=TankRightBM[i1]*tankColor[i];
		tank[i].dir=UP;
		tank[i].xmove=0;
		tank[i].ymove=0;
		tank[i].score=0;
		tank[i].treeshit=0;
		tank[i].upk=UpKeys[i];
		tank[i].downk=DownKeys[i];
		tank[i].leftk=LeftKeys[i];
		tank[i].rightk=RightKeys[i];
		tank[i].firek=FireKeys[i];
	}
}

main(int argc, char *argv[])
{
	int i;

	init();

	readconfig(); /* read from config file */
	gr_start_kbd_grab();


	/* main game loop, exits when esacape key pressed */
	while(configmenu()) {
      restart();
		while (!gr_keys[01]) {
			shots();
			dotanks();
		   if (newscreen) restart();
		}
		while (kbhit()) getch();
		mode(3);
   }

	CursorOn;
   gr_end_kbd_grab();

	while(bioskey(1)) bioskey(0);   /* clear the keyboard buffer */

	clrscr();
   return(1);

}