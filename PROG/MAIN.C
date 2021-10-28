/////////////////////////////////////
// Project Main.c
// Started April - 24 1996
// By: Franco Gaetan
/////////////////////////////////////

#include <stdio.h>
#include <dos.h>                // Make far Pointer
#include <conio.h>		// for GETCH
#include <alloc.h>
#include "main.h"
#include "anim.h"

////////////////////////////////////
// Global Variables
///////////////////////////////////

char far *screen = MK_FP(0xa000,0);             // Screen Pointer

int offset;					// Offset from Screen
int x,y;					// X & Y Coordinates
int val,rw,sp;					// All Coords
int j=2;						// Color Code   ******
int button,lso,xm,ym;				// Mouse Control
int rw=0,sp=0;					// Sprite Row and SP_Num
int allflg,chkdrw;				// Set my Flags Variables
int acxm,acym;					// Mouse Coords
int ny,val,oval,oy,width;
int memflag=0;

///////////////////////////////////
// End Global
//////////////////////////////////



///////////////////////////////////
//   External Variables
//////////////////////////////////

extern char *destin;
extern char far *st1;
extern char far *savearea;
extern int ext,rc,hc,lc;				// Return Value from Keyboard

//////////////////////////////////
//    End External
/////////////////////////////////


struct animshape animobjects[TOTALANIMS];


void main (void)
{
	Init_Mode();			// Set Mode X 320 x 200 256 Colours
	Init_Screen();			// Allocate 64K for Double Buffer
	initarray();
	PutColor();
	DrawGrid();


	while (ext != 256)
	{
		Get_Mouse(0);
		Calc_Coords();
		Check_Draw();

		Check_Color();

		gotoxy (4,4);
		printf("Sp:%d",sp);

		gotoxy (4,5);
		printf("Rw:%d",rw);

		gotoxy (4,6);
		printf("Max:%d",animobjects[rw].max);

		gotoxy (4,7);
		printf("Colour: %d",j);

		gotoxy (4,8);
		printf("Flag: %d",animobjects[rw].fshp[sp]->flag);

		if (bioskey(1))
		{
			rc = bioskey(0);
			hc = (rc>>8)&0xff;
			lc = rc&0xff;
		  //	printf("\n");
		  //    printf("%d %d \n",hc,lc);
		}

	if (hc == 1 && lc == 27)   // ESC Key & Exit
	{
		ext=256;
		printf("Exit\n");
	}

	if (hc == 59 && lc == 0)     		   // 'F1' Help Screen
	{
		nextshape(rw,sp);
		Close_Mode();
		helpscreen();
		Init_Mode();
		placeshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);
		PutColor();
		DrawGrid();
		hc=0;lc=0;
	}

	if (hc == 63 && lc == 0)      		  // 'F5' Save Row (current)
	{
		SaveRow();
		hc=0;lc=0;
	}


	if (hc == 22 && lc == 117)		// 'U' Undp Last Change
	{
		clearshape();
		UndoLastChange();
		nextshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);
		hc=0;lc=0;
	}


	if (hc == 30 && lc == 97)        	// 'A'Animate
	{
		nextshape(rw,sp);
		Animate();
		hc=0;lc=0;
	}



	if (hc == 34 && lc == 103)       	 // 'G'Draw Grid
	{
		DrawGrid();
		hc=0;lc=0;
	}

	if (hc == 68 && lc == 0)       		 // 'F-10' Debug Mode
	{
		Debug();
		hc=0;lc=0;
	}

	if (hc == 66 && lc == 0)       		 // 'F-8' Layout Mode
	{
		nextshape(rw,sp);
		layout_screen(rw,sp);
		hc=0;lc=0;
	}


	if (hc == 46 && lc == 99)       	 // 'C'Copy Shape
	{
		copyshape();
		hc=0;lc=0;
	}

	if (hc == 18 && lc == 101)       	 // 'E'Erase Shape
	{
		EraseObject();
		hc=0;lc=0;
	}

	if (hc == 33 && lc == 102)       	 // 'F'Flip Shape
	{
		nextshape(rw,sp);
		flipshape();
		nextshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);
		hc=0;lc=0;
	}

	if (hc == 50 && lc == 109)        	// 'M'Move Shape
	{
		DrawGrid();
		hc=0;lc=0;
	}

	if (hc == 73 && lc == 0 && rw >0)        // Page Up
	{

			nextshape(rw,sp);
			clearshape();
			cleargrid();
		if (rw >0)
		{
			sp=0;
			rw = rw - 1;
			gotoxy (10,25);
			placeshape(rw,sp);
			Draw_Data_to_Grid(rw,sp);
			hc=0;lc=0;
		}
	}



	if (hc == 81 && lc == 0 && rw < 9)        // Page Down			{
	{
		nextshape(rw,sp);
		clearshape();
		cleargrid();
		if (animobjects[rw].fshp[sp]->flag==1 && chkdrw ==1 )
		{
			animobjects[rw].max+=1;
			chkdrw=0;
		}


		if (rw < (TOTALANIMS-1))
		{
			rw = rw + 1;
			sp=0;
			placeshape(rw,sp);
			Draw_Data_to_Grid(rw,sp);
			hc=0;lc=0;
		}
	}

	if (hc == 47 && lc == 118)       	 // 'V'View Shapes
	{
		nextshape(rw,sp);
		draw_screen(rw,sp);
		hc=0;lc=0;
	}




	if (hc == 31 && lc == 115)       	 // 'S'Save Shapes
	{
		nextshape(rw,sp);
			if (animobjects[rw].max>0)
			{
				filewrite();
				placeshape(rw,sp);
				Draw_Data_to_Grid(rw,sp);
			}
			else
			{
				gotoxy (1,25);
				printf("Nothing to Save ");
			}

			hc=0;lc=0;

	}

	if (hc == 38 && lc == 108)      	  // 'L'Load Shapes
	{
		sp=0;
		gotoxy (1,25);
		printf("Loading ");
		fileread();
		placeshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);
		hc=0;lc=0;
	}




	if (hc == 74 && lc == 45 && sp >0)        // '-' Moved down 1 object
	{
		 hc=0;lc=0;
		 nextshape(rw,sp);
		 if (sp >0)
		 {
			 sp = sp - 1;
			 clearshape();
			 cleargrid();
			 placeshape(rw,sp);
			 Draw_Data_to_Grid(rw,sp);
			 gotoxy (1,25);         //       Calculate Start of gride 10 lines down
		 }


	}


	if (hc == 78 && lc == 43 && sp <9)        // + Moved over 1 object
	{
		hc=0;lc=0;
		nextshape(rw,sp);
		if (sp <10)
		{
			sp = sp + 1;
			clearshape();
			cleargrid();
			placeshape(rw,sp);
			Draw_Data_to_Grid(rw,sp);
		}
	}
	hc=0;lc=0;
}

	getch();


	farfree(destin);		// Return 64K to System
	Close_Mode();                   // Return to  Text Mode

}

