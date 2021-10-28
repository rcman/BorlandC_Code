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
void RestoreImage(int spx,int spy, int spritenum);
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
void Check_Location(int spx, int spy);


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
int spx=150;
int spy=112;
int spritenum=0;
		

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
	PutImage(spx,spy,60);
	copycentre((dest+0x2820), (screen+0x2820),64,154,64);
	Check_Location(spx, spy);

	while (ext != 256)

		{
	  // int CLocX=32;
	  //	int CLocY=32;



		if (bioskey(1))
		{
			rc = bioskey(0);
			hc = (rc>>8)&0xff;
			lc = rc&0xff;
										

		if (hc == 72 && lc == 0)        // Up Arrow
		{
		CLocY = CLocY + 1;

		if (CLocY<=100)
		{
		copypageup(dest+0,dest+0x140);
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
		copypagedn(dest+0xfa00,dest+0xf8c0);
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
		CLocX -=1;
		if (CLocX >1)
		{
		copypageRtoL(dest+1,dest+0);
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
		CLocX +=1;
		if (CLocX <100)
		{
			 copypageLtoR(dest+0xf9ff,dest+0xfa00);
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

int fileread(void)
{

	int s=0,i;
	struct seqfshape *p;

	if ((in = fopen("def1.std", "rb"))
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

void Check_Location(int spx, int spy)
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

		int block=0;
		int myval=0;
		int stx=0,sty=0;
		unsigned int dis=0;
		int nx,ny,ox,oy,n1,n2,nt;
		curval=0;
		myval = (spy*320)+spx;
		*(screen+myval)=43;
		for (i =0; i <20; i++)  // This calculate where in the X axis it is
		{
		 ox = i*16;
		 nx = spx - ox;
			if (nx <16 && nx >0)
				{
					sty = i;
				}

		 }
		 for (j=0;j<10;j++)		// This caclulate where in the Y axis it is
		 {
				// Distance 5120 

				oy = (j*16)*320;
				n1 = (spy+12)*320; 
				nt = 16 * 320;				// distance 5120   
				n2 = n1 - oy;
						if (n2 <= nt && n2 >0)
						{
							stx = j;
						}
		  }
			printf("%d  %d ",sty,stx);
		  getch();
		  
}
				  
void animate_man(void)
{


		// Order Cut BAckground
		// Place Man EOR
		//	Restore Background
		Check_Location(spx,spy);
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
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		  *(dest+y+offset)^=seqfshp[spritenum].shp[x*16+y];;
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

