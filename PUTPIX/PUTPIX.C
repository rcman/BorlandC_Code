
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mem.h>

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
void InitChain4(int);
int C4PutPixel(int, int , int, int );
void OutPixel(int , int , int );
void moveto(int , int );
void Plane(int);


//Global

char far *screen = MK_FP(0xa000,0);
char far *destin;

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

int Size;
int depth=200;

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


/*
void initarray(void)
{


	int c,sn,tempx,tempy,tempdir;


	randomize();
	for (c=0 ; c<numspr; c++)
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
			 exit(1);  /* terminate program if out of memory
		}


		PutImage(animobject[c].shipx, animobject[c].shipy, c);
		}

}

*/


void main(void)
{
	int m,i,j,k,xx,swav;
	int farin;

	Size =	80;
	depth =0;

	Init_Mode();
	InitChain4(80);

	moveto(0,200);

	for (j=0;j<200;j++)
	{
		for (m=1;m<=5;m++)
		{
			for (i=0;i<319;i++)
			{
		 for (k=0;k<=8;k++)
		 {
		      C4PutPixel(k+i,m, 1, 80);

		 }
	      }
	   }

	depth = depth - 1;
	   if (depth < 0)
	   {
		depth=200;
	   }
	moveto(0,depth);

	getch();
	}
	moveto(0,200);

	getch();



	Close_Mode();

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

   //	for (i=0 ; i<numspr ; i++)
    //	{
   //		farfree(shipanim[i].saveback);
    //	}
	// farfree(destin);



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


/*
void OutPixel(int x, int y, int col)

{
	// x = x coordinate
	// y = y coordinate
	// col = color


	asm {
	     .386
	     mov ah,0x0c
	     mov al,[col]
	     mov cx,[x]
	     mov dx,[y]
	     mov bx,[1]
	     int 10
	    }
}
*/

void InitChain4(int Size)
{

	asm {
	    mov    dx, 0x3c4   	//  { Port 3c4h = Sequencer Address Register }
	    mov    al, 0x04		//       { Index 4 = memory mode }
	    out    dx, al
	    inc    dx  		//  { Port 3c5h ... here we set the mem mode }
	    in     al, dx
	    and    al, not 0x08
	    or     al, 0x04
	    out    dx, al
	    mov    dx, 0x3ce
	    mov    al, 0x05
	    out    dx, al
	    inc    dx
	    in     al, dx
	    and    al, not 0x10
	    out    dx, al
	    dec    dx
	    mov    al, 0x06
	    out    dx, al
	    inc    dx
	    in     al, dx
	    and    al, not 0x02
	    out    dx, al
	    mov    dx, 0x3c4
	    mov    ax, (0x0f shl 0x08) + 0x02
	    out    dx, ax
	    mov    ax, 0xa000
	    mov    es, ax
	    sub    di, di
	    mov    ax, 0x0000 	// {0x8080}
	    mov    cx, 0x8000			// Change to hex
	    cld
	    rep    stosw        //    { Clear garbage off the screen ... }

	    mov    dx, 0x3d4
	    mov    al, 0x0e
	    out    dx, al
	    inc    dx
	    in     al, dx
	    and    al, not 0x28
	    out    dx, al
	    dec    dx
	    mov    al, 0x11
	    out    dx, al
	    inc    dx
	    in     al, dx
	    or     al, 0x28
	    out    dx, al

	    mov    dx, 0x3d4
	    mov    al, 0x0d
	    out    dx, al
	    inc    dx
	    mov    al, [Size]    //  { Size * 8 = Pixels across. Only 320 are visible}
	    out    dx, al
	    }



}

// Procedure C4PutPixel(X,Y : Word; Col : Byte); ASSEMBLER;
//   { This puts a pixel on the chain 4 screen }


int C4PutPixel(int x , int y, int Col, int Size)
{

	asm {
	    .386
	    mov   ax,[y]
	    xor   bx,bx
	    mov   bl,[Size]
	    imul  bx
	    shl   ax,1
	    mov   bx,ax
	    mov   ax, [x]
	    mov   cx, ax
	    shr   ax, 2
	    add   bx, ax
	    and   cx, 00000011b
	    mov   ah, 1
	    shl   ah, cl
	    mov   dx, 0x3c4    //              { Sequencer Register    }
	    mov   al, 2        //             { Map Mask Index        }
	    out   dx, ax
	    mov   ax, 0xa000
	    mov   es, ax
	    mov   al, [Col]
	    mov   es: [bx], al
	    }

}



// Procedure Plane(Which : Byte); ASSEMBLER;
//  { This sets the plane to write to in Chain 4}

void Plane(int Which)
{
	asm {
	    .386
	    mov	   al, 2
	    mov    ah, 1
	    mov     cl, [Which]
	    shl     ah, cl
	    mov     dx, 0x3c4      //            { Sequencer Register    }
	    out     dx, ax
	    }
}


// {컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
// procedure moveto(x, y : word);
//  { This moves to position x*4,y on a chain 4 screen }

       //	var o : word;
       //	begin
      //	o := y*size*2+x;

void moveto(int x, int y)
{
	int o;

	o = y * Size * 2 + x;

	asm {
	    .386
	    mov    bx, [o]
	    mov    ah, bh
	    mov    al, 0x0c
	    mov    dx, 0x3d4
	    out    dx, ax
	    mov    ah, bl
	    mov    al, 0x0d
	    mov    dx, 0x3d4
	    out    dx, ax
	    }
}



