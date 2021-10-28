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
void Check_Grid(int spx, int spy);
int Check_Pixel(int spx, int spy);
void AnimImage(int r,int c, int animx, int animy);
void SetUpXY(void);
void Animate(int r,int c, int animx,int animy, int cs);
void DrawNewLine(void);
void EraseSingleLine(void);
void check_code(void);

//Globals
int stx=0,sty=0;				// Grid Find X and Y
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
int MapEndx=19;
int MapStarty=0;
int MapEndy=10;
int spx=16;
int spy=48;
int spritenum=0;
int step=1;
int front,back,bottom,top;
int ky=0;
int tempx,tempy;
int down=0;
int up=0;
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
	int ts;
	int i,j;


	Init_Mode();
	initarray();
	Alloc_Vert_Screen();
	fileread();							// Read Background In 256 * 100 Shape
	fileread2();					// Read Structured Shapes Plus Attribs.
	Current_Screen();
	SetUpXY();

	CutImage(animobjects[0].animx,animobjects[0].animy,0);
	AnimImage(6,2, animobjects[0].animx,animobjects[0].animy);


//	copycentre((dest+0x2820), (screen+0x2820),64,154,64);
	copycentre(dest, screen,320,200,0);

//
  //	fshp[1].x=spx;
  //	fshp[1].y=spy;

	while (ext != 256)
	{

		if (bioskey(1))
		{
			rc = bioskey(0);
			hc = (rc>>8)&0xff;
			lc = rc&0xff;


						if (hc == 72 && lc == 0)        // Up Arrow
						{
						//layarr[sty][stx]!=28		This is a staircase
				//		up=1;
						Check_Grid(animobjects[0].animx, animobjects[0].animy);
						check_code();
						Animate(6,animobjects[3].currentshape, animobjects[0].animx,animobjects[0].animy,20);

						if (animobjects[3].currentshape==animobjects[3].max)
							{
								animobjects[3].currentshape=4;
							}
							else
							{
							ts = animobjects[3].currentshape;
							ts = ts + 1;
							animobjects[3].currentshape = ts;
							}

					tempx = animobjects[0].animx;
					tempy = animobjects[0].animy;
					animobjects[0].animox = tempx;
					animobjects[0].animoy = tempy;

					if(layarr[sty][stx+1]==28 || layarr[sty][stx-1]==28)
					{
					tempy = tempy - animobjects[3].animspeed;
					animobjects[0].animy=tempy;
					}

					gotoxy(20,2);
					printf("above %d",layarr[sty][stx-1]);
					gotoxy(20,4);
					printf("below %d",layarr[sty][stx+1]);
					gotoxy(20,3);
					printf("Left %d",layarr[sty-1][stx]);

				  gotoxy(23,10);
					printf("%d  %d ",sty,stx);


		hc=0;lc=0;
		}
					if (hc == 80 && lc == 0)        // Down Arrow
					{
						CLocY = CLocY - 1;

								if (CLocY >=1)
								{
								//copypagedn(dest+0xfa00,dest+0xf8c0);
							  //	copycentre(dest, screen,320,200,0);
								}


		hc=0;lc=0;
		}

				if (hc == 77 && lc == 0)        // Right Arrow
				{

				Check_Grid(animobjects[0].animx, animobjects[0].animy);
				check_code();
				Animate(6,animobjects[0].currentshape, animobjects[0].animx,animobjects[0].animy,20);

							if (animobjects[0].currentshape==animobjects[0].max)
							{
								animobjects[0].currentshape=2;
							}
							else
							{
							ts = animobjects[0].currentshape;
							ts = ts + 1;
							animobjects[0].currentshape = ts;
							}

					tempx = animobjects[0].animx;
					tempy = animobjects[0].animy;
					animobjects[0].animox = tempx;
					animobjects[0].animoy = tempy;

					gotoxy(20,7);
					printf("Front %d",layarr[sty+1][stx]);
					gotoxy(21,8);
					printf("below %d",layarr[sty][stx+1]);

					//if (sty==0)
					 //	{
					  //	sty=1;
					  //	}

					if(layarr[sty+1][stx]!=0 && down==0)
					{
					tempx = tempx + animobjects[0].animspeed;
					animobjects[0].animx=tempx;
					}
						if(layarr[sty][stx+1]!=0 && layarr[sty][stx]!=28)
						{
						down =1;
						tempy = tempy + animobjects[1].animspeed;
						animobjects[0].animy=tempy;
						}
							else
							{
							down=0;
							}

				  gotoxy(23,10);
					printf("%d  %d ",sty,stx);


			  hc=0;lc=0;
		}

				if (hc == 75 && lc == 0)        // Left Arrow
				{
				Check_Grid(animobjects[0].animx, animobjects[0].animy);
				check_code();
				Animate(6,animobjects[1].currentshape, animobjects[1].animx,animobjects[1].animy,20);

							if (animobjects[1].currentshape==animobjects[1].max)
							{
								animobjects[1].currentshape=0;
							}
							else
							{
							ts = animobjects[1].currentshape;
							ts = ts + 1;
							animobjects[1].currentshape = ts;
							}

					tempx = animobjects[0].animx;
					tempy = animobjects[0].animy;
					animobjects[0].animox = tempx;
					animobjects[0].animoy = tempy;

					if(layarr[sty][stx]!=0 && down==0 )
					{
					tempx = tempx - animobjects[1].animspeed;
					animobjects[0].animx=tempx;
					}

							if(layarr[sty][stx+1]!=0)
							{
							down=1;
							tempy = tempy + animobjects[1].animspeed;
							animobjects[0].animy=tempy;
							}
									else
									{
									down=0;
									}
					gotoxy(20,2);
					printf("Front %d",layarr[sty][stx]);
						gotoxy(20,4);
					printf("below %d",layarr[sty][stx+1]);

					gotoxy(23,10);
					printf("%d  %d ",sty,stx);

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

	free(layarr);

	Close_Mode();

}

void SetUpXY(void)
{
	// Object 0 Defined  Man Walk Right

	animobjects[0].animx=spx;
	animobjects[0].animox=spx;
	animobjects[0].animy=spy;
	animobjects[0].animoy=spy;
	animobjects[0].currentshape=2;
	animobjects[0].max=3;
	animobjects[0].animspeed=3;

	animobjects[1].animx=spx;
	animobjects[1].animox=spx;
	animobjects[1].animy=spy;
	animobjects[1].animoy=spy;
	animobjects[1].currentshape=0;
	animobjects[1].max=1;
	animobjects[1].animspeed=3;

	animobjects[3].animx=spx;
	animobjects[3].animox=spx;
	animobjects[3].animy=spy;
	animobjects[3].animoy=spy;
	animobjects[3].currentshape=4;
	animobjects[3].max=5;
	animobjects[3].animspeed=1;



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

	malloc(layarr[100][100]);	// Row & Coloumn






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

void EraseSingleLine(void)
{
 int i;
	for (i=0;i<200;i++)
	{
		*(dest+i*320)=0;
	}

}


void DrawNewLine(void)
{
	int ap=19*16;
	int dn=0;
	int cnt=0;
	int t,s,y;
	//long int clr;
	int x;
	float t1=0;
	offset=0;
	rw = 0;
	sp =0;

	// int MapStartx=0;
	// int MapEndx=19;
	// int MapStarty=0;
	//	int MapEndy=10;

	for (t=0;t<=9;t++)
	{
	  //	printf("%d",t);
	  //	getch();
		 curval = layarr[MapEndx][t];

			 if (curval >=0)
			 {
			 t1 = ((float)curval /10);
			 rw = t1;
			 t1 = t1 - rw;
			 t1 = t1 * 10;
			 sp = (int) (t1+.1);
			 }

		 //	 gotoxy(30,12);
		  //	 printf("%d",sp);
			 for (x=0;x<16;x++)
			{
				for (y=0;y<16;y++)
				{
						if (curval>=0)
						{
						*(dest+y+offset+ap+dn)=animobjects[rw].fshp[sp]->shp[x*16+y];
						}
							 //	else
							 //	{
							///	*(dest+y+offset+ap+dn)=0;
							//	}
				}
				offset = offset + 320;      //bytes to next line
			}
			cnt = cnt + 16;
			dn = cnt * 320;
			offset = 0;
				
			}
			offset=0;
			dn=0;
			cnt=0;
			ap=0;
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

		int i,j;
		int tmpi,tmpj;
		int above,below,rght,lft;

		int step=16;
		int block=0;
		int block1=0;

		int tblkx=0;
		int tblky=0;
		int myval=0;
		unsigned int dis=0;
		int op=(320*8)+8;
		int test;
		curval=0;
		spx=fshp[1].x;
		spy=fshp[1].y;


		myval = (spy*320)+spx;
		tmpi = myval;

		myval = myval + op;

		do {
			 block = ((16 * tblky+8)*320)+((tblkx*16)+8);  // this is block I'm checking centre
			 tblkx +=1;

				if (tblkx ==20)
					{
					tblkx=0;
					tblky +=1;
					}
						if (tblky ==10)
							{
								block1=1;
							}

							 dis = myval - block;

									if (dis <0)
									{
										dis = dis * -1;
									}

						if (dis <16 && dis > 1)
						{
								block1=1;
								above = layarr[tblky][tblkx];
								rght	= layarr[tblky][tblkx];
								lft = layarr[tblky][tblkx];
								gotoxy(5,25);
								printf("%d  %d %d %d %d %d %d",tblkx,tblky,layarr[tblkx][tblky],fshp[1].x=spx,rght,lft,dis);

									if (lft ==51 && ky==1)
									{
									allowl=1;
									}
										if (rght==51 && ky==2)
										{
										allowr = 1;
										}

						}

					} while(block1!=1);

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
			savedarea[spritenum].backgnd[x*16+y]=*(dest+y+offset);
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
			if (seqfshp[spritenum].shp[x*16+y] >0)
			{
		  *(dest+y+offset)=seqfshp[spritenum].shp[x*16+y];;
			}
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
			 *(dest+y+offset)=savedarea[spritenum].backgnd[x*16+y];
			i++;
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
			if (animobjects[r].fshp[c]->shp[x*16+y] >0)
			{
		  *(dest+y+offset)=animobjects[r].fshp[c]->shp[x*16+y];
			}

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

void Check_Grid(int spx, int spy)
{

	sty = spx /16;
	stx = spy/16;
}

void Animate(int r,int c, int animx,int animy,int cs)
{
	// r = Row
	// c = Column
	// animx = Animation x value
	// animy = Animation y value
	// cs = Current Shape
	// mx = max shape

	RestoreBack(animobjects[0].animox,animobjects[0].animoy,0);
	CutImage(animobjects[0].animx,animobjects[0].animy,0);
	AnimImage(r,c, animobjects[0].animx,animobjects[0].animy);
	copycentre(dest, screen,320,200,0);


}

void check_code(void)
{
	int a,b,c;
	a = sty *16;
	b = (stx *16)*320;
	c = a+b;
	*(dest+c)=69;

}

