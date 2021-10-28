// Program
// Date Start: April 1, 1996


#include "draw.h"
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include <process.h>
#include <mem.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>


#include "fade.h"

#include "draw.h"

#define TOTALSHAPE 100
#define TOTALANIMS 100

void CheckValues(void);
void copyfromltor(void);
void Init_Screen(void);
void initarray(void);
void filewrite(void);
int fileread(void);
void nextshape(void);
void calcnum(void);
int getch(void);
void copypage();
void CutImage(int saveareax, int savesreay, int spritenum, int s);
void PutImage(int spx,int spy, int spritenum );
void EORImage(int spx,int spy, int spritenum );
void RestoreBack(int spx,int spy, int spritenum, int s );
void Animate(void);
void Alloc_Screen(void);
void SyncToVerticalRetrace( void );
void draw_screen();
void MoveSprites(int spx, int spy, int s);
void FreeMem(void);
void draw_screen(void);
//void randomize(void);

char pal[768];
char pal2[768];

const char *image="satelite.raw";
const char *palf1="pal1.pal";
const char *palf2="pal2.pal";


char far *scrn  = MK_FP(0xa000,0);
char far *screen  = MK_FP(0xa000,0);
char far *destin = MK_FP(0xa000,0);

//unsigned char pal[256*3];

int start=40,send=48,s=40;
int xval=0;
int x=0,y=0,offset=0,val=50;
int numspr=30;
int spx=100;            // Pixels
int spy=100;
int xx=0;
int n;

//char far *savearea;


FILE *in, *out;

struct ship {
		int shipox;
		int shipoy;
		int shipx;
		int shipy;
		int oldspr;
		int curspr;
		int shipspeed;
		char far *spritemask;
		char far *savearea;

	 } shipanim[40];


struct fshape
	{

		int w,h;
		int n,c;
		int flag;
		int rowflag;
		char far shp[260];
					// TOTALSHAPE  =  20
	} fshp[TOTALSHAPE];


static char screens[12][20] ={
	{ 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,0x02, 0x03, 0x02, 0x02,0x02, 0x02, 0x02, 0x02 },
	{ 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,0x02, 0x03, 0x02, 0x02,0x02, 0x02, 0x02, 0x02 },
	{ 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,0x02, 0x03, 0x02, 0x02,0x02, 0x02, 0x02, 0x02 },
	{ 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,0x02, 0x03, 0x00, 0x00,0x00, 0x00, 0x00, 0x00 },
	{ 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,0x02, 0x02, 0x02, 0x02,0x02, 0x02, 0x02, 0x02 },
	{ 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,0x02, 0x02, 0x02, 0x02,0x02, 0x02, 0x02, 0x02 },
	{ 0x02, 0x02, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02,0x02, 0x02, 0x02, 0x02,0x02, 0x02, 0x02, 0x02 },
	{ 0x02, 0x02, 0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x02, 0x03, 0x00,0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00 },
	{ 0x02, 0x02, 0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x02, 0x03, 0x02,0x02, 0x02, 0x02, 0x02,0x02, 0x02, 0x02, 0x02 },
	{ 0x02, 0x02, 0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x02, 0x03, 0x02,0x02, 0x02, 0x02, 0x02,0x02, 0x02, 0x02, 0x02 },
	{ 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x02,0x02, 0x02, 0x02, 0x02,0x02, 0x02, 0x02, 0x02 },
	{ 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02,0x02, 0x02, 0x02, 0x02,0x02, 0x02, 0x02, 0x02 },

	};




void main(void)
{



	Init_Mode();

	fileread();

	readstuff(palf1,&pal,768);
	readstuff(palf2,&pal2,768);

	draw_screen();

      //	do
     //
      //	{

    //	copyfromltor();
     //	copypage();
	getch();

	fade_out_screen(&pal);
	// bright_in_screen(&pal);

	// bright_out_screen(&pal);

	getch();

     //	} while (!kbhit());

	Close_Mode();

}


void initarray(void)
{


	int c,sn;

	randomize();

	for (c=0 ; c<numspr; c++)
	{
		shipanim[c].shipx=random(315);
		shipanim[c].shipy=random(170);
		shipanim[c].curspr=40;


		shipanim[c].shipox=shipanim[c].shipx;
		shipanim[c].shipoy=shipanim[c].shipy;
		shipanim[c].shipspeed=random(5)+1;
		shipanim[c].oldspr=40;

	    if ((shipanim[c].savearea = (char far  *) farmalloc(260l)) == NULL)
	    {
		     printf("Not enough memory to allocate buffer\n");
		     exit(1);  /* terminate program if out of memory */
	    }

		PutImage(shipanim[c].shipx, shipanim[c].shipy, s);
		}

}


void FreeMem(void)
{
	int i;

	for (i=0 ; i<numspr ; i++)
	{
		farfree(shipanim[i].savearea);
	}

	farfree(destin);


}

void Init_Screen(void)
{

       //	if ((savearea = (char far  *) farmalloc(350)) == NULL)
      //	{
      //	printf("Not enough memory to allocate buffer\n");
		//	exit(1);  /* terminate program if out of memory */
      //	}


}




int fileread(void)
{

   int s=0,i;
	struct fshape *p;

   if ((in = fopen("sean1.std", "rb"))
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


void Alloc_Screen(void)
{

	destin = (char far *) farmalloc(64000l);

	if (destin == NULL)
	{
		 printf("Not enough memory to allocate screen buffer. Press any key to Exit \n");
		 exit(1);  /* terminate program if out of memory */
	}
	_fmemset(destin, 0, 64000l);

}



void Animate(void)
{

	int i=0,j=0;
	int vz=0;


	if (s >= send)
		s= 40;

		for (i=0;i<16;i++)
		{

			for (j=0;j<16;j++)
			{
				*(destin+j+vz+xval)=fshp[s].shp[i*16+j];
			}
			vz = vz + 320;

		}
	       //       gotoxy(35, 12);
	      //        printf("Number %d ",a);
		s = s + 1;
		xval +=1;
		vz=0;

//
//              getch();





}

void copypage(void)
{

//      printf("Start ASM");
	asm {
		 push ds
		 push es
		 push si
		 push di
		 mov ax,0xa000
		 mov es,ax
		 mov ds,ax

		 //mov si, 0x9fff
		 //mov di, 0xa000

		 mov si, 0xf9ff
		 mov di, 0xfa00

		 //lds si, [source]
		 //les di, [destin]

		 mov cx, (16*320/4)
		 std
		 db 0x66
		 rep
		 movsw

		 pop di
		 pop si
		 pop es
		 pop ds
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


void CutImage(int spx,int spy, int spritenum, int s)
{
	int i=0;
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		*(shipanim[spritenum].savearea+i)=*(screen+y+offset);
		i++;
		}

		offset = offset + 320;      //bytes to next line
	}


}







void PutImage(int spx,int spy,int spritenum)
{
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		if (fshp[spritenum].shp[x*16+y] >0)
		{
		 *(destin+y+offset)=fshp[spritenum].shp[x*16+y];

		}
		else
		{
		y = y + 1;
		}

		}
		offset = offset + 320;      //bytes to next line
	}

}

void EORImage(int spx,int spy,int spritenum)
{
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		if (fshp[s].shp[x*16+y] >0)
		{
		 *(screen+y+offset)^=fshp[spritenum].shp[x*16+y];
		}
		else
		{
		y = y + 1;
		}

		}
		offset = offset + 320;      //bytes to next line
	}

}

void RestoreBack(int spx,int spy, int spritenum, int s)
{
	int i=0;
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		  *(screen+y+offset)=*(shipanim[spritenum].savearea+i);
		  i++;
		}
		offset = offset + 320;      //bytes to next line
	}

}

void draw_screen(void)
{
	int ap=0;
	int dn=0;
	int cnt=0;
	int t=0,s=0;

	for (t=0;t<12;t++)
	{
		for (s=0;s<20;s++)
		{
			if (screens[t][s]==1)
			{
				for (x=0;x<16;x++)
				{
					for (y=0;y<16;y++)
					{
					*(scrn+y+offset+ap+dn)=fshp[1].shp[x*16+y];;
					}
					offset = offset + 320;      //bytes to next line
				}
				offset = 0;
				ap = ap +16;
				//getch();
			}
			if (screens[t][s]==2)
			{
				for (x=0;x<16;x++)
				{
					for (y=0;y<16;y++)
					{
				*(scrn+y+offset+ap+dn)=fshp[0].shp[x*16+y];

					}
				offset = offset + 320;      //bytes to next line
				}
				offset = 0;
				ap = ap + 16;
			       //       getch();
			}

			if (screens[t][s]==3)
			 {
				 for (x=0;x<16;x++)
				{
					 for (y=0;y<16;y++)
					{
					*(scrn+y+offset+ap+dn)=fshp[28].shp[x*16+y];
					}
					 offset = offset + 320;      //bytes to next line
				 }
				 offset = 0;
				 ap = ap +16;
				 //getch();
			 }
			if (screens[t][s]==0)
			 {
				 for (x=0;x<16;x++)
				{
					 for (y=0;y<16;y++)
					{
					*(scrn+y+offset+ap+dn)=0;
					}
					 offset = offset + 320;      //bytes to next line
				 }
				 offset = 0;
				 ap = ap +16;
				 //getch();
			 }



		}
		cnt = cnt + 15;
		dn = cnt * 320;
		offset=0;

	}
}


void copyfromltor(void)
{

	int rs=58879;
	int ls=58560;
	int i,dwn=0;
	int pix=0;



	for (i=0;i<=16;i++)
	{

	   pix=*(screen+rs+dwn);
	   *(screen+ls+dwn)=pix;
	   dwn=dwn+320;
	}
	dwn=0;




}

void CheckValues(void)
{

	int rs=58879;
	int ls=58560;
	int i,dwn=0;
	int pix=1;



	for (i=0;i<16;i++)
	{

 	delay(50);

	 //  pix=*(screen+ls+dwn);
	   *(screen+rs+dwn)=pix;
	   dwn=dwn+320;
	}
	dwn=0;




}

int readstuff(const char *filename,void far *buf,unsigned length)
{
   int handle, bytes;


   if ((handle =
      sopen(filename, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) == -1)
   {
      printf("Error Opening File\n");
      exit(1);
   }

   if ((bytes = read(handle, buf, length)) == -1) {
      printf("Read Failed.\n");
      exit(1);
   }
   return 0;
}


/*

SavePalette PROC NEAR        ; Save entire palette into an array
    cli                      ; Clear interrupts
    lea     bp,Parray        ; bp points to offset Palette array
    mov     dx,03c7h         ; Read register
    xor     al,al            ; Set al to 0 (start reading at color 0)
    out     dx,al            ; Give info to VGA
    mov     dx,03c9h         ; Data register
    mov     cx,768           ; Save all colors (256*3)
Saving:
    in      al,dx            ; Get what's in the register (read)
    and     al,00111111b     ; Mask of the upper 2 bits (value=0..63)
    mov     byte ptr [bp],al ; Save value into array
    mov     al,0             ; Set value to 0
    mov     byte ptr [bp+768],al  ; And save into second layer of array
    inc     bp               ; Point to next cel in aray
    loop    Saving           ; And loop while cx > 0
    sti                      ; Enable interrupts again
    ret
SavePalette ENDP
  */
