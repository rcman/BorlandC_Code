
// Defender style game like Datastorm
// Date Start: April 23, 1995
// Revision: May 7, 1995
// Latest Rev: Feb 14, 1996
// Single Bottom Screen Scroll
// Task List    Init Screen
//              Load Objects
//              Init Enemies
//              Calculate Coords
//              Move Enemies
//              Scroll Screen
//**********************************************************
//    PUT (x, SIN(ya * (3.14159267# / 180)) * 60 + 100), Box%, XOR
//        LOCATE 1, 1: PRINT SIN(ya); ya; yac
//        yao = ya
//        xo = x
//        PUT (x, SIN(ya * (3.14159267# / 180)) * 60 + 100), Box%, XOR
//        ya = ya + yac
//        IF ya > 360 THEN ya = 0: ' ya = 360
//        'IF ya < 0 THEN yac = 1: ya = 0
//        x = x + .3
//        FOR t = 1 TO 10: NEXT t
//        PUT (xo, SIN(yao * (3.14159267# / 180)) * 60 + 100), Box%, XOR
//        GOTO mainloop
//

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mem.h>
//#include <complex.h>



//#include <conio.h>
//#include <graphics.h>
#pragma inline
void Init_Mode(void);
void Close_Mode(void);
void CutImage(int saveareax, int savesreay, int spritenum);
void PutImage(int spx,int spy, int spritenum );
void RestoreBack(int spx,int spy, int spritenum );
void getch(void);
void initarray(void);
void copypage(char *source, char *destin);
int kbhit(void);
void initscreen(void);


int getkey(void);
void far *farmalloc(unsigned long int);

// int farmalloc(long int);
void farfree(void far *block);
//Global

char far *screen = MK_FP(0xa000,0);


int scn=64000;                                  // 16 x 16 array
int spary=260;

static char ship1[16][16] = {

    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x17, 0x17, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x17,0x17, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x17, 0x17, 0x17, 0x17,0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x17, 0x17, 0x17, 0x17, 0x17,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x17, 0x17, 0x17, 0x17, 0x17, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x17, 0x17, 0x17, 0x17, 0x17, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x17, 0x17, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },


  };

void *buffer;
void *image;
unsigned int imsize;
int top = 290;
int bottom = 35;
int rbottom = 300;
int lbottom = 145;
int spx=100;            // Pixels
int spy=100;
int spritenum=0;
int lp;
int x,y,xy1,xy,fl,tm;
int z=0;
int offset = 0;
int numspr=10;

struct animshape {
		int active;
		int nonactive;
		int animwidth;
		int animheight;
		int animox;
		int animoy;
		int animx;
		int animy;
		int prox;
		int direction;
		int animspeed;
		int currentshape;
		int oldshape;
		char far *bufferarea;


	 } animobjects[TOTALSHAPE];

void initscreen(void)
{



	printf(" ");
	destin = (char far *) farmalloc(64000l);

//      printf("%p",destin);

	if (destin == NULL)
		{
			 printf("Not enough memory to allocate screen buffer\n");
			 //Close_Mode();
			 exit(1);  /* terminate program if out of memory */

		}
		_fmemset(destin, 0, 64000);


}



void initarray(void)
{


	int c,sn,tempx,tempy,tempdir;


	randomize();
	for (c=0 ; c<TOTALSHAPE; c++)
	{
		tempx=random(319);
		tempy=random(169);
		tempdir = random(2);

		animobject[c].shipx=tempx;
		animobject[c].shipy=tempy;
		animobject[c].direction=tempdir;


		shipanim[c].shipspeed=random(3)+1;
		 if ((shipanim[c].saveback = (char far  *) farmalloc(260l) == NULL)
		{
			 printf("Not enough memory to allocate sprite buffer\n");
			 exit(1);  /* terminate program if out of memory */
		}


		PutImage(animobject[c].shipx, animobject[c].shipy, c);
		}

}


void main(void)
{
	int i,xx,swav;


	Init_Mode();
	// initscreen();
	initarray();




}






void Init_Mode(void)
{
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

	/* free memory */

	for (i=0 ; i<numspr ; i++)
	{
		farfree(shipanim[i].saveback);
	}
	// farfree(destin);



}







void PutImage(int spx,int spy,int spritenum)
{
	offset=spy*320+spx;


	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
  //              *(destin+y+offset)^=ship1[x][y];
   *(screen+y+offset)^=ship1[x][y];
		}
		offset = offset + 320;      //bytes to next line
	}

}



void copypage(char *source, char *destin)
{
//      printf("Start ASM");
	asm {
	    .386
	    push ds
	    cld
	    lds si, [source]
	    les di, [destin]
	    mov cx, 16000
	    rep movsd
	    pop ds
	    }
}




int getkey()
{

	getchar();

}



void moveobjects(void)
{
	do
	{
		 for (xx=0; xx<numspr; xx++)
		  {

		 spritenum=xx;
			z=xx;
			z=spritenum;

			  spx = shipanim[xx].shipx;
			  spy = shipanim[xx].shipy;;
			  shipanim[xx].shipox = spx;
			  shipanim[xx].shipoy = spy;

			  spx += shipanim[xx].shipspeed;

			  if (spx >= 319)
			  {

			spx=1;
			}
			  shipanim[xx].shipx = spx;

			  PutImage(shipanim[xx].shipox,spy,spritenum);
			  //CutImage(spx,spy,spritenum);
			  PutImage(spx,spy,spritenum);
		}
//                copypage(destin,screen);

	  //    getkey();

		//  for (tm=0 ;tm<9350 ; tm++)
	   //   {
	   //          printf("");
	  //    }
	} while (!kbhit());




}

