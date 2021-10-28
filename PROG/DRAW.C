// Drawing Program
// Date Start: June 1, 1995
// Revision: April 28 - 1996
// DRAW V 6

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

#pragma inline
#define TOTALSHAPE 12
#define TOTALANIMS 12
#define TOTALOBJECTS 20

void Init_Screen(void);
void initarray(void);
void CutImage(void);
void PutImage(void);
void RestoreBack(void);
void PutColor(void);
void BoxImage(void);
// void BoxMask(void);
int getch(void);
int Get_Mouse(int command);
void DrawCursor(void);
void DrawBox(void);
void DrawMask(void);
void Calc_Coords(void);
void Check_Color(void);

char far *screen = MK_FP(0xa000,0);

int lc,rc,hc;
int nff=0;
int flag=1;
int spy=100;
int spritenum=0;
int lp;
int x,y,xy1,xy,fl,tm,width,oy,ym,xm,button,chkdrw=0;
int j=2;                // start off color
int z=0;
int offset = 0;
int numspr=10;


int nx,ny,val,mspeed,oldoffset,oldy,oldval,tm,oval,ptflag;
int c,b,st,ac,step,tim;
int inc=5;
int lso=0;
int acxm,acym;          // Actuall Y Mouse and X mouse
char far *point;
char *p;
char far *st1;
char undoarea[260];
int sp=0,rw=0;
int allflg=0;
int layout=0;
char sc[20];
int ga=47,gb=61,gc=144,gd=126;
int sval=5; 				// step value for draw
int chgpnt=0;
int memflag=0;
int svalue=0;		// save Value
int boxflag=0;



FILE *in, *out, *outdata, *indata, *std;


extern char box[6][6];
extern char plus[7][7];
extern char minus[7][7];
extern char sixteen[17][17];
extern char boxmask[6][6];


struct savebg
	{
	char far backgnd[256];

	} savedarea[TOTALOBJECTS];



struct fshape
	{

		int w,h;
		int n,c;
		int flag;
		int rowflag;
		char far shp[260];

					// TOTALSHAPE  =  20
	};



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


void main(void)
{
	int ext=0;

	sp=0;                   // sprite value
	ptext();


	Init_Mode();
	Init_Screen();
	initarray();
	PutColor();
	DrawGrid();

//	Get_Mouse(0);
//	Calc_Coords();
//	BoxImage();

	gotoxy (29,23);
	printf("F1 - Help ");

//      if (!Get_Mouse(1))
//     {                   7
//	  printf("No Mouse Driver Present\n");
 //	  exit(1);
  //    }

       //       gotoxy (1,25);         //       Calculate Start of gride 10 lines down
 //     printf("Sprite %d  Row %d ",sp,rw);

      

	}
	farfree(st1);
//      farfree(undoarea);
	farfree(savearea);


	Close_Mode();

}

void Calc_Coords(void)
{


	   xm/=2;
	   mspeed=2;
	   xm = xm / sval;
	   ym = ym / sval;
	   acxm = xm;                   // Coords for little box
	   acym = ym;
	   xm = xm * sval+1;
	   ym = ym * sval;

	   ny = ym * 320;
	   val = xm + ny;
	   oval = val;

	   tim = oval;

	   if (svalue == val) 		// Then I know I moved
		boxflag=1;
	   else
	   {
	   svalue=val;
	   boxflag=0;
	   }




}

void Init_Screen(void)
{

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

	if ((q=(char far *)farmalloc(sizeof (struct savebg)*TOTALOBJECTS))==NULL)
	{
		printf("Could Not Allocate Memory for Save Area Objects\n");
		exit(1);
	}




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

	// anims[i].fshp[j]->w=15;
}



void BoxImage()

{

	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)
		{
		    *(screen+y+offset+val)=box[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	oy=width;



}

/*
void BoxMask()

{

	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)

		{
		  *(screen+y+offset+oldval)^=boxmask[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}

	offset=0;
}
*/

int Get_Mouse(int command)
{
      int found=0;

      if (command)
      {

	      asm     {
		      push    ds
		      mov     ax,0
		      int     0x33
		      mov     [found],ax
		      pop     ds
		      }

		  return found;
	  }

	asm     {
		push    ds
		mov     ax,3
		mov     bx,3
		int     0x33
		mov     [ym],dx
		mov     [xm],cx
		and     bx,7                        // if 0 then no button
		mov     [button],bx                 // if 1 the left button
		pop     ds
		}

	return 0;
}

void CutImage(void)
{
	int i=0;

	for (x=0;x<18;x++)
	{
		for (y=0;y<18;y++)
		{
			*(savearea+i)=*(screen+y+offset+val);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}

	offset=0;
}

void PutImage(void)
{

	for (x=0;x<4;x++)
	{
		for (y=0;y<4;y++)
		{
		  *(screen+y+offset+val)=j;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;

}

void RestoreBack(void)
{
	int i=0;

	for (x=0;x<18;x++)
	{
		for (y=0;y<18;y++)
		{
			 *(screen+y+offset+val)=*(savearea+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
}






void Colors(void)
{
	long int b;

	 for (c=16;c<32;c++)
	 {
		for (b=0;b<64000;b++)
		{
		*(screen+b)=c;
		}
	 }
}
void Check_Draw(void)
{


		if (button==0)
		{

			CutImage();
				if (layout==0)
				{
				for (tm=0;tm<1300;tm++)
				BoxImage();
				}
			else
			{
			for (tm=0;tm<1300;tm++)
			BigBox(rw,sp);
			}

			RestoreBack();

			lso=0;

		}
		if (button==1)

		{

			if (xm >=ga && ym >=gb && ym <= gc && xm <=gd)
			{
				if (animobjects[rw].fshp[sp]->flag==0)
					animobjects[rw].max+=1;
				animobjects[rw].fshp[sp]->flag=1;
			      //
				allflg=1;
				chkdrw=1;
				if (lso==0)
				{
					SaveLastImage();
					lso=1;
				}

				if (layout==1)
				{
					BigBox(rw,sp);
				}
				else
				{
				PutImage();
				*(screen + acxm + acym * 320 + 32260) = j;
				}
			}

			    //	}

		}
	     //	oxm = xm;


}



void drawmem(void)
{

	int b,c,loc;

	loc = 320*100;
	offset=0;
	for (b=0;b<6;b++)
	{
		for(c=0;c<6;c++)
		{
		if (box[b][c]>0)
			{
			*(screen+c+offset+loc)^=box[b][c];
			*(screen+c+offset+320+32260) = box[b][c];
			}

//                     getch();
		}
		offset=offset+320;
	}
	offset=0;

	getch();

}





void drawshape(int rw , int sp)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<x;i++)

	    {
		for (j=0;j<y;j++)
		{

			*(screen+j+vz)=animobjects[rw].fshp[sp]->shp[i*16+j];

		}

		vz = vz + 320;

	    }




}





int filewrite(void)
{

   int i,numobj,numrow;
   char label[20];
   char infst[20];
   char stan[20];

   gotoxy (1,23);
   printf("Please enter Filename: ");
   fflush(stdin);  /* flush the input stream in case of bad input */

   scanf("%8s",label);
   strcpy(infst, label);
   strcpy(stan, label);

   strcat(label,".dat");
   strcat(infst,".inf");
   strcat(stan,".std");


   if ((std = fopen(stan, "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open Standard file.\n");
	  return 1;
   }


   if ((out = fopen(infst, "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open output file.\n");
	  return 1;
   }

	if ((outdata = fopen(label, "wb"))
		   == NULL)
		{
		  fprintf(stderr, "Cannot open output file.\n");
		  return 1;
		}

		fwrite(animobjects,sizeof(animobjects),1,outdata);


	for (numrow=0;numrow<10;numrow++)
	{
		for (numobj=0;numobj<10;numobj++)
		{
			if (animobjects[numrow].fshp[numobj]->flag==1)
			{

			  fwrite(animobjects[numrow].fshp[numobj],272l,1,out);
			  fwrite(animobjects[numrow].fshp[numobj]->shp,256,1,std);
			}
		}
	}

   fclose(out);
   fclose(outdata);
   fclose(std);
   return 1;

}

int fileread(void)
{

	int i,j,numobj,numrow;
	char far *p;
	char label[20];
	char infst[20];

	gotoxy (1,23);
	printf("Please enter Filename: ");
	fflush(stdin);  /* flush the input stream in case of bad input */

	scanf("%8s",label);

	strcpy(infst, label);

	strcat(label,".dat");
	strcat(infst,".inf");


	if ((in = fopen(infst, "rb"))
	   == NULL)
	{
	  fprintf(stderr, "Cannot open input file.\n");
	  return 1;
	}

	if ((indata = fopen(label, "rb"))
		   == NULL)
		{
		  fprintf(stderr, "Cannot open output file.\n");
		  return 1;
		}

		fread(animobjects,sizeof(animobjects),1,indata);

	p = st1;

	for (i=0;i<10;i++)
	{
		for (j=0;j<10;j++)

			{
			animobjects[i].fshp[j]=(struct fshape *)p;
			p+=sizeof(struct fshape);

			}
	}


	for (numrow=0;numrow<10;numrow++)
	{
	    //  printf ("%d\n",animobjects[numrow].max);
	      if (animobjects[numrow].max >9)
		animobjects[numrow].max=9;

		for (numobj=0;numobj<=animobjects[numrow].max;numobj++)
	      //	getch();
		{
		fread(animobjects[numrow].fshp[numobj],272l,1,in);
		}
	}
//      getch();
//        getch();






   fclose(in);
   fclose(indata);


   return 1;



}




void copyshape(void)
{

    int cp,dn,i;

    gotoxy (1,25);
    printf("Source :");
    cp=getch()-'0';

    if (cp < 0 || cp > 9)
	printf("Object out of Row Range");
    else
    {
	gotoxy (1,25);
	printf("Destin :");
	dn=getch()-'0';
		if (dn <0 || dn >9)
			{
			gotoxy (1,25);
			printf("Object destination out of Range");
			}
		else
			if (animobjects[rw].fshp[dn]->flag==1)
				{
				gotoxy (1,25);
				printf("Destination Contains Data");
				}
				else
				{
				for (i=0;i<259;i++)
				{
				animobjects[rw].fshp[dn]->shp[i]=animobjects[rw].fshp[cp]->shp[i];
				}
				animobjects[rw].fshp[dn]->flag=1;
				animobjects[rw].max+=1;
				placeshape(rw,sp);
				Draw_Data_to_Grid(rw,sp);

				}

    }





}



void EraseObject(void)
{

	clearshape();
	cleargrid();
	if (animobjects[rw].max>0 && animobjects[rw].fshp[sp]->flag != 0)
		animobjects[rw].max--;
	animobjects[rw].fshp[sp]->flag=0;
	nextshape(rw,sp);


}


void SaveLastImage(void)
{

	// *(savearea+i)=*(screen+y+offset+val);
	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
			undoarea[j+i*16]=*(screen+j+36430+vz);

		}
		vz = vz + 320;
	    }

	    lso=1;

}





void CutObject(int rw, int sp)
{
	int i=0;
	int tempx,tempy;

	for (i=0;i<TOTALOBJECTS;i++)
	{
	    tempx = animobjects[rw].animx;
	    tempy = animobjects[rw].animy * 320 ;
	    offset = tempx + tempy;

	    for (x=0;x<16;x++)
	    {
		for (y=0;y<16;y++)
		{
		savedarea[i].backgnd[i*16+j]=*(screen+offset+y);
		}
		offset = offset + 320;      //bytes to next line
	    }

	offset=0;
	}
}

void PutObject(void)
{
	// put objects on screen
	// save object to oldobject coords
	// add speed to object
	// add animation to next frame
	// check if object is at end of anim - if yes set to 0
	int i;

	for (i=0;i<TOTALOBJECTS;i++)
	{
	    for (x=0;x<16;x++)
	    {
		for (y=0;y<16;y++)
		{
		  *(screen+y+offset+val)=animobjects[rw].fshp[sp]->shp[i*16+j];
		}
		offset = offset + 320;      //bytes to next line
	    }
	offset=0;
	}
}

void RestoreObject(void)
{
	int i=0;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			 *(screen+y+offset+val)=*(savearea+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
}




void DrawRow(int rw)
{

	int x;
	int ap=0;
	int dn=0;
	int cnt=0;
	int s,v;


	offset=160*320;


		for (s=0;s<=9;s++)
		{
			if (animobjects[rw].fshp[s]->flag==1)
			{
				for (x=0;x<16;x++)
				{
					for (y=0;y<16;y++)
					{
					*(screen+y+offset+ap+dn)=animobjects[rw].fshp[s]->shp[x*16+y];
					}
					offset = offset + 320;      //bytes to next line
				}

				offset = 160*320;
				ap = ap +18;
				//getch();
			}
		}
		cnt = cnt + 18;
		dn = cnt * 320;
		offset=0;
		ap=0;






}

void BigBox(int rw, int sp)
{

	for (x=0;x<17;x++)
	{
		for (y=0;y<17;y++)
		{
		    //if (layout==1)
		   // *(screen+y+offset+val)^=sixteen[x][y];
		   // else
		  //  {
		    *(screen+y+offset+val)=animobjects[rw].fshp[sp]->shp[x*16+y];;
		  //  }


		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	oy=width;



}







void Check_Select(void)
{
	int ext;

	if (bioskey(1))
	{
		rc = bioskey(0);
		hc = (rc>>8)&0xff;
		lc = rc&0xff;
		  //	printf("\n");
		  //    printf("%d %d \n",hc,lc);

		gotoxy (20,24);
		printf("SP= %d RW= %d",sp,rw);

		if (hc == 74 && lc == 45)   // - Key
		{
			if (sp <=0 && rw >0)
			{
			sp=9;
			rw = rw -1;
			DrawRow(rw);
			}
			else
			{
			if (sp >0)
			{
			sp = sp -1;
			hc=0; lc=0;
			}
			}

		}

		if (hc == 78 && lc == 43)        // '+'
		{
			if (sp >8 && rw <=9)
			{
			sp=0;
			rw = rw + 1;
			DrawRow(rw);
			}
			else
			{
			sp = sp + 1;
			hc=0;lc=0;
			}
		}


	}

}


void draw_plus_minus(void)

{

	int i,j,vz;

	vz=0;

	for (i=0;i<7;i++)
		{
		for (j=0;j<7;j++)
		{

			*(screen+j+57600+vz)=minus[i][j];
			*(screen+j+57610+vz)=plus[i][j];

		}
		vz = vz + 320;

	    }


}

void DrawPnt(void)
{


	*(screen+y+offset+val)=0x0b;






}

/*
void ClearAllData(void)
{
	int i,j;
//	for (i=0;i<10;i++)
		for (j=0;j<10;j++)
 //		      animobjects[j]=0;



}

  */