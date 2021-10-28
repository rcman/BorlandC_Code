
#include "anim.h"
#include "main.h"

#pragma inline

extern struct animshape animobjects[TOTALANIMS];
extern int button;
extern int lso,xm,ym;				// Last Saved Object
extern int rw,sp;
extern int allflg,chkdrw;
extern char far *screen;
extern char far *savearea;
extern int acxm,acym;
extern int ny,val,oval;					// Mouse Variables
extern int j;
extern int offset,oy,width;

int tm;
int mspeed;					// Mouse Speed

static char box[6][6] = {
    { 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, },
    { 0x0b, 0x00, 0x00, 0x00, 0x00, 0x0b, },
    { 0x0b, 0x00, 0x00, 0x00, 0x00, 0x0b, },
    { 0x0b, 0x00, 0x00, 0x00, 0x00, 0x0b, },
    { 0x0b, 0x00, 0x00, 0x00, 0x00, 0x0b, },
    { 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, },

      };


void Check_Draw(void)
{

		Get_Mouse(0);
		Calc_Coords();


		if (button==0)
		{

			CutImage();
			for (tm=0;tm<1100;tm++)
			BoxImage();
			PutImage();

			RestoreBack();
			lso=0;
		 //	if(lso == 0 && chkdrw ==1)
		   //           SaveLastImage();

		}

		if (button==1)

		{

			if (xm >=47 && ym >=61 && ym <= 144 && xm <=126)
			{
				if (animobjects[rw].fshp[sp]->flag==0)
					animobjects[rw].max+=1;
				animobjects[rw].fshp[sp]->flag=1;
			      //
				allflg=1;
				chkdrw=1;
				if (lso==0)
				{
//					SaveLastImage();
					lso=1;
				}

				//lso=0;

				PutImage();
				*(screen + acxm + acym * 320 + 32260) = j;
			}



		}

//                      oxm = xm;
}

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


void Calc_Coords(void)
{
	   int sval=5;
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

	  // tim = oval;

//	   gotoxy (1,25);         //       Calculate Start of gride 10 lines down
//         printf("%d %d",oxm,xm);



}

void BoxImage()

{


	int x,y;

	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)
		{
		    *(screen+y+offset+val)^=box[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	oy=width;
}

