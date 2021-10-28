// By: Franco Gaetan
// Date Start: Dec 2, 1996

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <bios.h>
#include <conio.h>
#include "draw.h"

#define TOTALSHAPE 100
#define TOTALANIMS 10
#define TOTALOBJECTS 20

void PlaceLine(int line);
void checkletter(int line);
int  getch(void);
void PutImage(int spx,int spy);
int fileread(void);
void CheckImage(int line);
void SyncToVerticalRetrace( void );



//Global

char far *screen = MK_FP(0xa000,0);
char far *destin;

int spx=0;            // Pixels
int spy=0;
int x,y,xy1,xy,fl,tm,s,t,l;
unsigned int offset = 0;

//////////////////////////

char *pc[20];

int leng=0;
int totalleng=0;
int spaceleftb=0;
int spaceleftt=0;
int bottom=190;	// bottom let add X to it to get shape
int line=0;
int let=0;
char p;
int v=0;


int nline=0;
int pixelcount=0;
int linecount=0;
int spritenum=0;



FILE *in, *out, *outdata, *indata, *std, *map, *mapin;


struct fshape
	{

		int w,h;
		int n,c;
		int flag;
		int rowflag;
		char far shp[260];
					// TOTALSHAPE  =  20
	} fshp[TOTALSHAPE];


struct animshape {
		int active;
		int animwidth;
		int animheight;
		int animox;
		int animoy;
		int animx;
		int animy;
		int prox;
		int animspeed;
		int currentshape;
		int oldshape;
		int max;
		int row;

		struct fshape *fshp[TOTALSHAPE];        // 10 Pointers to data

	 } animobjects[TOTALANIMS];


void main()
{
	int i;

	pc[0] = "this is element";
	pc[1] = "welcome to gc software";
	pc[2] = "conactfranco";
	pc[3] = "httpfranco";
	pc[4] = "thanks to sean and all ";
	pc[5] = "you bastered";
	pc[6] = "hi to all my friends";

	Init_Mode();
	fileread();				// draw first line

	checkletter(line);		// Check leng of Line and Start
	PlaceLine(line);

		do {

	if (linecount==60)
		{
		linecount=0;
		pixelcount=0;
		nline = nline + 1;
		line = line + 1;
		checkletter(line);
		}


	if (pixelcount <=35)
	{
	CheckImage(line);
	}

	      //	SyncToVerticalRetrace();
	      //	copypageup();
		PlaceLine(line);
		spy=spy-1;
		delay(10);
		linecount = linecount + 1;


	}while(nline!=14);

	Close_Mode();
}


int fileread(void)
{

	int s=0,i;
   struct fshape *p;

	if ((in = fopen("mylet4.std", "rb"))
		 == NULL)
	{
		fprintf(stderr, "Cannot open input file.\n");
		return 1;

	}

	i=0;
	for (s=0,p=&fshp[s];s<TOTALSHAPE && feof ;s++,p=&fshp[s],i=0)

	while (i<256)
		{
	*(p->shp+i)=fgetc(in);
  //      printf("%x",*(p->shp+i));
	i++;
		}

	fclose(out);

	return 1;

}

void initscreen(void)
{



	destin = (char far *) farmalloc(64000l);

	if (destin == NULL)
		{
			 printf("Not enough memory to allocate screen buffer\n");
			 exit(1);  /* terminate program if out of memory */

		}
		_fmemset(destin, 0, 64000);


}


void CutImage(int spx,int spy, int spritenum)
{
	int i=0;
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		      //	*(shipanim[spritenum].saveback+i)=*(screen+y+offset);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}


}







void PutImage(int spx,int spy)
{
	offset=spy*320+spx;

	for (x=0;x<3;x++)
	{
		for (y=0;y<16;y++)
		{
		  *(screen+y+offset)^=fshp[2].shp[x*16+y];
		}
		offset = offset + 320;      //bytes to next line
	}

}

void RestoreBack(int spx,int spy, int spritenum)
{
	int i=0;
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
//			 *(screen+y+offset)=*(shipanim[spritenum].saveback+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}

}


void SyncToVerticalRetrace( void )
  {
  // If we happen to be in the middle of a vertical retrace
  // period wait until its over.

  while( inp( 0x3da ) & 8 )
    ;

  // Wait until we begin vertical retrace.

  while( !(inp( 0x3da ) & 8) )
	 ;
  }



void CheckImage(line)
{


	int i,x,y,over=0;


	if (offset <=64000)
	{


	offset=spy*320+leng;
						// Letter a = 60

	for (i=0;i<=totalleng;i++)
	{
		p = *(pc[line]+i);		// ACS value of a = 97
		v=(int)p;

		let = v - 97;                   //
		let = let + 60;
	       //	printf("let= %d",let);
	      //	getch();
		if (let ==-5)		// is it a space
			let=36;


		for (x=0;x<8;x++)
			{
		       //	delay(33);
				pixelcount=pixelcount+1;

				for (y=0;y<8;y++)
				{
				*(screen+y+offset+over)=fshp[let].shp[x*16+y];
				 }
				 offset = offset + 320;      //bytes to next line
			 }

			 over = over + 9;
			 offset = spy*320+leng;
			 spy=190;
			 let=0;
			}
		}

}


void checkletter(int line)
{

	int i;


	leng=0;


	// p = *(pc[0]+j);

	do {

		p = *(pc[line]+leng);
		leng +=1;
		v=(int)p;
	      //	printf("v = %d",v);
	      //  getch();

	}while(p!=0);

	totalleng = leng-2;

	leng = leng / 2;
	leng = leng * 8;
	leng = 160 - leng;


}


//10 charter 8 bit + 2
// centre 160
// devide leng of string / 2
// multiple by 8
// subtract 160

void PlaceLine(line)
{

	char p;
	int i,v;
	int over=0;
	int f=0;


	spy = 205;		// Offscreen so you can't see

	offset=spy*320+leng;
	animobjects[0].animx=leng;

	for  (i=0;i<=leng;i++)

	animobjects[i].animx=leng+over;
	animobjects[i].animy=spy;

	  {
	  p = *(pc[line]+i);
	  v = p - 97;			// Letter 'a' is 97
	  f = v + 60;

	      for (x=0;x<8;x++)
	      {
		   for (y=0;y<8;y++)
		   {
		   *(screen+y+offset+over)^=fshp[f].shp[x*16+y];
		   }
	       offset = offset + 320;      //bytes to next line
	       }

	       over = over + 9;
	       offset = 0;
	 }


}
