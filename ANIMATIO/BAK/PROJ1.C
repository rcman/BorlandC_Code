// New Project Started from Scratch
// Date Started: Dec 6, 1996
// Written By Franco Gaetan
// This Source in NOT Free
//

#include "draw.h"
#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conio.h>

// Defines

#define TOTALSHAPE 10
#define TOTALANIMS 10
#define TOTALOBJECTS 20


// Prototypes

void main(void);
void CutImage(int spx,int spy, int spritenum);
void PutImage(int spx,int spy, int spritenum);
void RestoreBack(int spx,int spy, int spritenum);
int getch(void);
int fileread(void);
int fileread2(void);
void ClearData(void);
void initarray(void);
//void copypagedn();
void draw_screen(void);
void copypagedn(char far *source, char far *destin);
void copypageup(char far *source, char far *destin);
void copypageLtoR(char far *source, char far *destin);
void copypageRtoL(char far *source, char far *destin);
void Current_Screen(void);
void Check_Location(void);
int Check_Pixel(int spx, int spy);
void SetUpXY(void);
void AnimImage(int r,int c, int animx, int animy);

//Globals

int layarr[100][100];		// Row & Coloumn
int curval=0;					// Current Value
char far *p;
char far *st1;
char far *screen = MK_FP(0xa000,0);

int rw=0,sp=0;
unsigned int offset=0;
//////////////////Keys

int rc = 0;
int hc = 0;
int lc = 0;
int ext =0;

int allowr=0;
int allowl=0;
int allowu=0;
int allowd=0;

///////////////////////////
/// Man Sprite
//
int ManSpR[2]={60,61};
int ManSpL[2]={62,63};
int ManClm[2]={64,65};
int MapStartx=0;
int MapEndx=20;
int MapStarty=0;
int MapEndy=12;
int spx=100;
int spy=112;
int spritenum=0;
int step=1;
int front,back,bottom,top;
int ky=0;
int above=0,below=0,rght=0,lft=0;
int i,j;
int tmpi,tmpj;
int block=0;
int myval=0;


///////////////////////
// Screen Location
int CLocX=32;
int CLocY=32;
//////////////////////////////
// Virtual Screen
/////////////////////////////
// Start 32*320+32 Top Left
// Bottom right is 32*320-32 = 10208
//
// VirStart = 40960 + 10272	= 51232
// VirEnd   = 64000 - 10208     = 53792
// A000 = 40960
// FA00 = 64000
//
int VirSoff=10272;
char *sw;			// Screen Width
char *ba;			// Bytes Accross

char ScnStart=0xa000+10272;


char label[20]; 	// Filenames
char infst[20];
char stan[20];
char smap[20];

// External Variables
extern char far *dest;
extern int VirStart;


// Structures

struct savebg

	{
	int bgflag;
	char far backgnd[256];

	} savedarea[TOTALOBJECTS];


struct seqfshape
	{

		int w,h;
		int n,c;
		int flag;
		int rowflag;
		char far shp[260];
					// TOTALSHAPE  =  20
	} seqfshp[100];

struct fshape
	{

		int x,y;
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




// Files
FILE *in, *mapin,*indata;


// **************************************************************************

void main(void)
{

	Init_Mode();
	initarray();
	Alloc_Vert_Screen();
	fileread();							// Read Background In 256 * 100 Shape
	fileread2();					// Read Structured Shapes Plus Attribs.
	Current_Screen();
	SetUpXY();
//	AnimImage(2,0, animobjects[0].animx,animobjects[0].animy);
//	getch();

//	PutImage(animobjects[0].animx,animobjects[0].animy,30);

	copycentre((dest+0x2820), (screen+0x2820),64,154,64);

	while (ext != 256)

		{
	  //	int CLocX=32;
	  //	int CLocY=32;



		if (bioskey(1))
		{
			rc = bioskey(0);
			hc = (rc>>8)&0xff;
			lc = rc&0xff;


		if (hc == 72 && lc == 0)        // Up Arrow
		{

//		Check_Location();

		if (allowu==1)
		{
		spy = fshp[1].y;
		spy = spy -1;
		fshp[1].y=spy;
		allowu=0;
	  //	PutImage(fshp[1].x,fshp[1].y,60);
			}


		CLocY = CLocY + 1;

		if (CLocY<=100)
		{
	 //	copypageup(dest+0,dest+0x140);
		copycentre((dest+0x2820), (screen+0x2820),64,154,64);
		}
		if (CLocY >101)

			{
			CLocY=100;
			}

		hc=0;lc=0;

		}
			if (hc == 80 && lc == 0)        // Down Arrow
		{

		CLocY = CLocY - 1;


		if (CLocY >=1)
		{
		//copypagedn(dest+0xfa00,dest+0xf8c0);
		copycentre((dest+0x2820), (screen+0x2820),64,154,64);
		}
		if (CLocY <0)
			{
			CLocY=0;
			}
		hc=0;lc=0;
		}

		if (hc == 77 && lc == 0)        // Right Arrow
		{
		Check_Location();

		CLocX -=1;
		if (CLocX >1)
		{
	 //	copypageRtoL(dest+1,dest+0);
		copycentre((dest+0x2820), (screen+0x2820),64,154,64);
		}
			if (CLocX <1)
				{
					CLocX=0;
				}

		hc=0;lc=0;
		}

		if (hc == 75 && lc == 0)        // Left Arrow
		{
		Check_Location();

		CLocX +=1;
		if (CLocX <100)
		{
		//	 copypageLtoR(dest+0xf9ff,dest+0xfa00);
			 copycentre((dest+0x2820), (screen+0x2820),64,154,64);
		}
			if (CLocX >100)
				{
					CLocX=100;
				}

		hc=0;lc=0;
		}





			if (hc == 1 && lc == 27)   // ESC Key & Exit
		{
		ext=256;
		printf("Exit\n");
		}

		}



	}

	Free_Vert_Screen();
	Close_Mode();

}

void SetUpXY(void)
{

	animobjects[0].animx=100;
	animobjects[0].animox=100;
	animobjects[0].animy=112;
	animobjects[0].animoy=112;

}



int fileread(void)
{

	int s=0,i;
	struct seqfshape *p;

	if ((in = fopen("sample5.std", "rb"))
		 == NULL)
	{
		fprintf(stderr, "Cannot open input file.\n");
		return 1;

	}

	i=0;
	for (s=0,p=&seqfshp[s];s<100 && feof ;s++,p=&seqfshp[s],i=0)

	while (i<256)
		{
	*(p->shp+i)=fgetc(in);
  //      printf("%x",*(p->shp+i));
	i++;
		}

	fclose(in);

	return 1;

}



int fileread2(void)
{

	int i,j,numobj,numrow;
	char far *p;

  //	ClearData();		// Clear the Array

	gotoxy (1,22);
	printf("Please enter Filename: ");
	fflush(stdin);  /* flush the input stream in case of bad input */

	scanf("%8s",label);

	strcpy(infst, label);
	strcpy(smap, label);

	strcat(label,".dat");

	strcat(infst,".inf");
	strcat(smap, ".map");

	if ((mapin = fopen(smap, "rb"))
	 == NULL)
	 {
	 fprintf(stderr, "Cannot open Standard file.\n");
	  return 1;
	 }

	if ((in = fopen(infst, "rb"))
		== NULL)
	{
	  fprintf(stderr, "Cannot open input file.\n");
	  return 1;
	}

	if ((indata = fopen(label, "rb")) == NULL)
	{
		  fprintf(stderr, "Cannot open output file.\n");
		  return 1;
	}

		for(i=0;i<100;i++)
		{
		  for(j=0;j<100;j++)
		  {
		  fscanf(mapin,"%d",&layarr[j][i]);
		  }

		}
	fread(animobjects,sizeof(animobjects),1,indata);


	p = st1;

	for (i=0;i<=9;i++)
	{
		for (j=0;j<=9;j++)
		{
		 animobjects[i].fshp[j]=(struct fshape *)p;
		 p+=sizeof(struct fshape);
		}

	}


	for (numrow=0;numrow<=9;numrow++)
	{

		for (numobj=0;numobj<TOTALSHAPE;numobj++)
		{
		 fread(animobjects[numrow].fshp[numobj],272l,1,in);
		}

	}

	fclose(mapin);
	fclose(in);
	fclose(indata);


	return 1;



}


void ClearData(void)
{
	int t,s,x,y;

	for (t=0;t<=9;t++)
	{
		for (s=0;s<=9;s++)
		{
			for (x=0;x<16;x++)
			{
				for (y=0;y<16;y++)
				{
				animobjects[t].fshp[s]->shp[x*16+y]=0;
				}
			}
		}

	}




}



void initarray(void)
{

	int i,j;
	//char far *q;

	if ((p=(char far *)farmalloc(sizeof (struct fshape)*TOTALSHAPE*TOTALANIMS))==NULL)
	{
		printf("Could Not Allocate Memory for Objects\n");
		exit(1);
	}

	_fmemset(p,0,sizeof(struct fshape)*TOTALSHAPE*TOTALANIMS);

	st1 = p;


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
	for (i=0;i<=9;i++)
	{
		savedarea[i].bgflag=0;
	}
					// int layarr[100][50];	// Row & Coloumn
	for (i=0;i<100;i++)
	{
		for (j=0;j<50;j++)
			{
			layarr[i][j]=-1;	// Row & Coloumn
			}
	}






}


void copypagedn(char far *source, char far *destin)
{

	asm {
		 push ds
		 push es
		 push si
		 push di
		 les di,[source]
		 lds si,[destin]
		// mov si, 0xf8c0
		// mov di, 0xfa00
		 mov cx, 16000
		 std
		 db 0x66
		 rep movsw
		 pop di
		 pop si
		 pop es
		 pop ds
		 }
}



void copypageup(char far *source,char far *destin)
{

	asm {
		 push ds
		 push es
		 push si
		 push di
		 les 	di,[source]
		 lds	si,[destin]
	  //	 mov si, 0x140
	  //	 mov di, 0
		 mov cx, 16000
		 cld
		 db 0x66
		 rep movsw

		 pop di
		 pop si
		 pop es
		 pop ds
		 }
}



void copypageLtoR(char far *source,char far *destin)
{


	asm {
		 push ds
		 push es
		 push si
		 push di
		 les 	di,[source]
		 lds	si,[destin]
	 //	 mov si, 0xf9ff
	//	 mov di, 0xfa00
		 mov cx, 16000
		 std
		 db 0x66
		 rep movsw

		 pop di
		 pop si
		 pop es
		 pop ds
		 }
}


void copypageRtoL(char far *source, char far *destin)
{
	asm {
		 push ds
		 push es
		 push si
		 push di
		 les 	di,[source]
		 lds	si,[destin]
	 //	 mov si, 1
	 //	 mov di, 0
		 mov cx, 16000
		 cld
		 db 0x66
		 rep movsw

		 pop di
		 pop si
		 pop es
		 pop ds
		 }
}




void draw_screen(void)
{
	int ap=0;
	int dn=0;
	int cnt=0;
	int t,s,y,x;
  //	long int clr;

				 
	for (t=0;t<=9;t++)
	{
		for (s=0;s<=9;s++)
		{
			if (animobjects[t].fshp[s]->flag==1)
			{
				for (x=0;x<16;x++)
				{
					for (y=0;y<16;y++)
					{
					*(screen+y+offset+ap+dn)=animobjects[t].fshp[s]->shp[x*16+y];
					}
					offset = offset + 320;      //bytes to next line
				}

				offset = 0;
				ap = ap +18;
			}
		}
		cnt = cnt + 18;
		dn = cnt * 320;
		offset=0;
		ap=0;


	}

}




void Current_Screen(void)
{
	int ap=0;
	int dn=0;
	int cnt=0;
	int t,s,y;
	//long int clr;
	int x;
	float t1=0;

	rw = 0;
	sp =0;

	for (t=0;t<=10;t++)
	{
		for (s=0;s<=19;s++)
		{

			 curval = layarr[s][t];
			// printf("%d",curval);
			// getch();

			 if (curval >=0)
			 {
			 t1 = ((float)curval /10);
			 rw = t1;
			 t1 = t1 - rw;
			 t1 = t1 * 10;
			 sp = (int) (t1+.1);
			 }


			 for (x=0;x<16;x++)
			{
				for (y=0;y<16;y++)
				{
				if (curval>=0)
				{
				*(dest+y+offset+ap+dn)=animobjects[rw].fshp[sp]->shp[x*16+y];
				}
				else
				{
				*(dest+y+offset+ap+dn)=0;
				}

				}
				offset = offset + 320;      //bytes to next line
				}

				offset = 0;
				ap = ap +16;
			}
		cnt = cnt + 16;
		dn = cnt * 320;
		offset=0;
		ap=0;


		}


	}



void DrawMap(void)
{

	int i,j;

	for (i=0;i<100;i++)
	{
		for (j=0;j<50;j++)
			{
			layarr[i][j]=-1;	// Row & Coloumn
			}
	}

}

void Check_Location(void)
{
	// int ManSpR[2}=(70,71);
	// int ManSpL[2]={72,73};
	// int ManClm[2]={74,75};
	// int MapStartx=0;
	// int MapEndx=19;
	// int MapStarty=0;
	//	int MapEndy=10;
	// spx and spy are guy coords

		int step=16;
		unsigned int dis=0;
		int op=(320*8)+8;
		int test;
		curval=0;


		for (i=0;i<10;i++)
		{
			for (j=0;j<20;j++)
			{

			 block = ((j * 16)+(i*16)*320);  // this is block I'm checking centre
			 tmpi = block+(8*320+8);


			 myval = ((((animobjects[0].animy/16)*16)*320))+((animobjects[0].animx)+(8*320+8));

			 dis = myval - tmpi;
//			 printf("%d",dis);


			 if (dis <=0)
				{
				dis = dis * -1;
				}

			 if (dis <16)
				{
				// printf("%d",dis);
					*(dest+myval)=43;
					*(dest+tmpi)=2;

					above = layarr[i][j];
					below = layarr[i+1][j];
					rght	= layarr[i][j+1];
					lft	= layarr[i][j];
				 //	printf("Right %d",rght);

				 j=21;
				 i=11;


						if (lft ==51)
						{
							spx = animobjects[0].animx;
							spx = spx - 1;
							AnimImage(2,0, animobjects[0].animx,animobjects[0].animy);
//							PutImage(spx,spy,30);
							animobjects[0].animx=spx;

						}
						if (rght==51)
						{
							spx = animobjects[0].animx;
							spx = spx + 1;
							AnimImage(2,0, animobjects[0].animx,animobjects[0].animy);
							//							PutImage(spx,spy,30);
							animobjects[0].animx=spx;
						}

						if	(below==51)
						{
							allowd = 1;
						}
						if	(above==51)
						{
							allowu =1;
						}
						}

					}

		}

}

void animate_man(void)
{


		// Order Cut BAckground
		// Place Man EOR
		//	Restore Background
		//Check_Location(spx,spy);
		CutImage(spx, spy, spritenum);
		PutImage(spx, spy, spritenum);







}

void CutImage(int spx,int spy, int spritenum)
{
	int i=0,x,y;
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			*(savedarea[spritenum].backgnd)=*(dest+y+offset);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}


}

void PutImage(int spx,int spy, int spritenum)
{
	int x,y;
	offset=(spy*320)+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		  *(dest+y+offset)^=seqfshp[spritenum].shp[x*16+y];;
		}
		offset = offset + 320;      //bytes to next line
	}

}


void AnimImage(int r,int c, int animx,int animy)
{
	int x,y;
	offset=(animy*320)+animx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		  *(screen+y+offset)^=animobjects[r].fshp[c]->shp[x*16+y];;
		}
		offset = offset + 320;      //bytes to next line
	}

}


void RestoreBack(int spx,int spy, int spritenum)
{
	int i=0,x,y;
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			 *(dest+y+offset)=*(savedarea[spritenum].backgnd+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}

}
// Check Pixel Method.

int Check_Pixel(int spx, int spy)
{

	int i=1;
 // Check the pixel infront, behind, below, above
  //	offset = (spy*320)+spx;
	front = ((spy*320)+spx)-1;
	back = ((spy*320)+spx) + 16;
	bottom = offset +(16*320);
	top = offset -(1*320);

	// check

	*(savedarea[1].backgnd+i)=*(dest+top);
	*(savedarea[2].backgnd+i)=*(dest+bottom);
	*(savedarea[3].backgnd+i)=*(dest+back);
	*(savedarea[4].backgnd+i)=*(dest+front);

	top = *(savedarea[1].backgnd+i);
	bottom = *(savedarea[2].backgnd+i);
	back = *(savedarea[3].backgnd+i);
	front = *(savedarea[4].backgnd+i);
 
  //	if (top ==0)
 //	{
		// allow jump
 //	}

  //	if (back ==0)
  //	{
		// Allow walk and fall
  //		spx=fshp[1].x;
	  //	spy=fshp[1].y;
	 //	spy =spy + 1;
  //		spx = spx + 1;
	//	fshp[1].x=spx;
	 //	fshp[1].y=spy;

  //	}
	if (front ==0 && bottom==0 && ky==2)
	{
		// Allow walk and Fall
		spx=fshp[1].x;
		spy=fshp[1].y;
		spy =spy + 1;
		spx = spx - 1;
		fshp[1].x=spx;
		fshp[1].y=spy;
		ky=0;
	}
	if (front ==0 && bottom!=0 && ky==2)
	{
		// Allow walk and Fall
		spx=fshp[1].x;
	  //	spy=fshp[1].y;
	  //	spy =spy + 1;
		spx = spx - 1;
		fshp[1].x=spx;
	  //	fshp[1].y=spy;
	  ky=0;
	}

  //	if (bottom == 0 )
  //	{
  //		spy=fshp[1].y;
  //		spy = spy + 1;
  //		fshp[1].y=spy;

		// Just Fall
//	 }
	
	if (back==0 && ky==1 && bottom ==0)
	{
	  //	printf("Back");
	  spx=fshp[1].x;
	  spy=fshp[1].y;
	  spy = spy + 1;
	  spx = spx + 1;
	  fshp[1].y=spy;
	  fshp[1].x=spx;
		ky=0;
	}

	
	if (back==0 && ky==1 && bottom !=0)
	{
	  //	printf("Back");
	  spx=fshp[1].x;
		spx = spx + 1;
		fshp[1].x=spx;
		ky=0;
	}

		PutImage(fshp[1].x,fshp[1].y,60);

  //	printf("%d",spx);
  //	printf("%d ",*(savedarea[1].backgnd+i));
  //	printf("%d ",*(savedarea[2].backgnd+i));
  //	printf("%d ",*(savedarea[3].backgnd+i));
  //	printf("%d ",*(savedarea[4].backgnd+i));
  //	getch();
  return(spx);





}
