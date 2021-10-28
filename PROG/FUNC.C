#include <conio.h>
#include <stdio.h>
#include <alloc.h>
#include <process.h>
#include "main.h"
#include "anim.h"

#pragma inline

///////////////////////////////////////////
// Global Variables
///////////////////////////////////////////

char far *destin;
char far *st1;          // save memory array pointer
char far *savearea;	// Self Explanitory
int ext,rc,hc,lc;				// Return Value from Keyboard

///////////////////////////////////////////

void Close_Mode(void);
void far * far _fmemset(void far *s, int c, size_t n);

//////////////////////////////////////////
// Externals
extern int ym,button;
extern int val;
extern int j;
extern char *screen;
extern struct animshape animobjects[TOTALANIMS];
extern char box[6][6];
extern char plus[7][7];
extern char minus[7][7];
extern char sixteen[17][17];
extern char boxmask[6][6];
extern int sp,rw;
extern int chkdrw;



//////////////////////////////////////////


void ptext(void)
{


    clrscr();
    cprintf("\r\n");
    textcolor(4);
    cprintf("Draw 96\r\n");
    cprintf("Copyright 1996(C)\r\n");
    textcolor(11);
    cprintf("Program Written By:\r\n");
    cprintf("Franco Gaetan & Sean Godsell\r\n");
    cprintf("This Program is NOT FREE\r\n");
    cprintf("Enter Password:");
    getch();



}

void Init_Mode(void)
{
	printf("Initialize 320 x 200 256 Colours\n");
	asm {
		mov ax,0x13
		int 0x10
	     }
}

void Close_Mode()
{

	asm {
		mov ax,0x03
		int 0x10
		}
//        printf("Graphics Screen Closed Thank You\n");

}


void helpscreen(void)
{


    clrscr();
    cprintf("\r\n");
    textcolor(4);
    cprintf("Help Screen\r\n");
    cprintf("\r\n");
    textcolor(11);
    cprintf("Keys \r\n");
    cprintf("   C       - Copy Object\r\n");
    cprintf("   E       - Erase Object\r\n");
    cprintf("   F       - Flip Object\r\n");
    cprintf("   T       - Turn Object\r\n");
    cprintf("   +       - Move Forward one Object\r\n");
    cprintf("   -       - Move Backward One Object \r\n");
    cprintf("   PgUp    - Move Up One Row\r\n");
    cprintf("   PgDn    - Move Down One Row\r\n");
    cprintf("   S       - Save Data\r\n");
    cprintf("   L       - Load Data\r\n");
    cprintf("   Q       - Quit Program\r\n");
    cprintf("   V       - View All Objects \r\n");
    cprintf("   M       - Move the Object \r\n");
    cprintf("   U       - Undo Last Change\r\n");
    cprintf("   A       - Animate to and from an Object \r\n");
    cprintf("   F1      - Help Screen \r\n");
    cprintf("\r\n");
    cprintf("   Press Any Key for Draw ");
    getch();



}


void CustomGrid(void)
{

	int dn,ht,st,b,c,offset,step;
	int calcx,calcy,dif,a;
	step=0;

	gotoxy (1,23);
	printf("Width of Grid ");

	scanf("%d",&dn);

	if (dn <0 || dn >32)
	{
		gotoxy (1,25);
		printf("Grid Cannot Exceed 32 blocks");
	}
	else
		{
		gotoxy (1,23);
		printf("Height of Grid  ");
		scanf("%d",&ht);

		if (ht <0 || ht >32)
			{
			gotoxy (1,25);
			printf("Height Cannot Exceed 32 blocks ");
			}

	else
	{
	gotoxy (1,25);
	printf("                        ");

	// **********************************************************
	calcx = dn;
	calcy = ht;

	offset=0;
	st = 64 * 320+10;
	dif = st;

	for (a=0;a<ht*dn;a++)
	{
	   for (b=0;b<ht;b++)
	   {
		for (b=0;b<5;b++)             // 60 Lines high and 60 accross
		{
		*(screen+st+b+offset)=15;
		}
		offset = offset + 320*5;

	  }
	st = dif +5;
	offset=0;
	}
	st=64 * 320+10;                 //start location

	getch();


	  for (c=0;c<dn*ht;c++)
	  {

		for (b=0;b<5;b++)             // 60 Lines high and 60 accross
		{
		*(screen+b+st+offset+step)=15;
		offset = offset + 319;
		}
		offset = 0;
		step = step + 5;
	  }

	offset=0;
	step=0;
	}
	}

}


void Init_Screen(void)
{

	//destin = (char far *) farmalloc(64000l);

	//if (destin == NULL)
	 //	{
	 //		 Close_Mode();                   // Return to  Text Mode
	 //		 printf("Not enough memory to allocate screen buffer. Press any key to Exit \n");
	 //		 getch();
	 //		 exit(1);  /* terminate program if out of memory */
	//
	//		}
	//	_fmemset(destin, 0, 64000);

	if ((savearea = (char far  *) farmalloc(350)) == NULL)
	{
	printf("Not enough memory to allocate buffer\n");
	exit(1);  /* terminate program if out of memory */
	}

}

void initarray(void)
{

	int i,j,s;
	char far *p;
	char far *q;

	if ((p=(char far *)farmalloc(sizeof (struct fshape)*TOTALSHAPE*TOTALANIMS))==NULL)
	{
		printf("Could Not Allocate Memory for Objects\n");
		exit(1);
	}

	_fmemset(p,0,sizeof(struct fshape)*TOTALSHAPE*TOTALANIMS);

	st1 = p;

//      if ((q=(char far *)farmalloc(sizeof (struct savebg)*TOTALOBJECTS))==NULL)
//      {
//              printf("Could Not Allocate Memory for Save Area Objects\n");
//              exit(1);
 //     }




	for (i=0;i<TOTALANIMS;i++)
	{
		for (j=0;j<TOTALSHAPE;j++)
			{
			animobjects[i].fshp[j]=(struct fshape *)p;
			p+=sizeof(struct fshape);
			animobjects[i].animx=100;
			animobjects[i].animy=100;

			}
	}

}

void DrawGrid(void)
{
	int st,c,b,offset=0,step=0;

	st = 64 * 320+50;
	for (c=0;c<17;c++)
	{
		for (b=0;b<80;b++)             // 60 Lines high and 60 accross
		{
		*(screen+b+offset+st)^=23;
		}
		offset = offset + 320*5;
	}
	offset=0;
	st=64 * 320+50;                 //start location

	for (c=0;c<17;c++)
	{

		for (b=0;b<80;b++)             // 60 Lines high and 60 accross
		{
		*(screen+b+offset+st+step)^=23;
		offset = offset + 319;
		}
		offset = 0;
		step = step + 5;
	}
	offset=0;
	step=0;

}

void PutColor()
{
	int ad,j,n,v,x,y,offset=0;
	v=0;
	n=0;
	ad=0;
	for (j=0;j<256;j++)
	{
	   for (x=0;x<4;x++)
	   {
		for (y=0;y<4;y++)
		{
		  *(screen+y+ad+offset+n+321)=j;
		}
		offset = offset + 320;      //bytes to next line
	   }
	 //  getch();
	 ad=ad + 5;                     // distance between colors

	 if (ad > 314)                  // if at end go to next line
	 {

		v = v + 5;
		ad = 0;
		n = 320 * v;


	 }
	 offset =0;

	 }
}

void Check_Color(void)
{
	// Colors are divisable by 2 since they are 4 wide
	// and 4 high
	// start at 0
	// increment by 5
	int cl=320*2+2;
	int x,y,offset=0;


	if (ym <= 19
	 && button==1)
	 {
	j = *(screen +val+cl);          // turn the block in to the color
					// 'J' is the color
	for (x=0;x<6;x++)               // draw the boax around the color
	{
		for (y=0;y<6;y++)
		{
		    *(screen+y+offset+val-1)=box[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	}

}




