#include <conio.h>
#include <stdio.h>

#pragma inline

extern char *screen;

void ptext(void)
{


    clrscr();
    cprintf("\r\n");
    textcolor(4);
    cprintf("Draw 96\r\n");
    cprintf("Copyright 1996(C)\r\n");
    textcolor(11);
    cprintf("Program Written By:\r\n");
    cprintf("Franco Gaetan & Sean Godsell\r\n");
    cprintf("This Program is NOT FREE\r\n");
    cprintf("Enter Password:");
    getch();



}

void Init_Mode(void)
{
        printf("Initialize 320 x 200 256 Colours\n");
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
        printf("Graphics Screen Closed Thank You\n");

}


void helpscreen(void)
{


    clrscr();
    cprintf("\r\n");
    textcolor(4);
    cprintf("Help Screen\r\n");
    cprintf("\r\n");
    textcolor(11);
    cprintf("Keys \r\n");
    cprintf("   C       - Copy Object\r\n");
    cprintf("   E       - Erase Object\r\n");
    cprintf("   F       - Flip Object\r\n");
    cprintf("   T       - Turn Object\r\n");
    cprintf("   +       - Move Forward one Object\r\n");
    cprintf("   -       - Move Backward One Object \r\n");
    cprintf("   PgUp    - Move Up One Row\r\n");
    cprintf("   PgDn    - Move Down One Row\r\n");
    cprintf("   S       - Save Data\r\n");
    cprintf("   L       - Load Data\r\n");
    cprintf("   Q       - Quit Program\r\n");
    cprintf("   V       - View All Objects \r\n");
    cprintf("   M       - Move the Object \r\n");
    cprintf("   U       - Undo Last Change\r\n");
    cprintf("   A       - Animate to and from an Object \r\n");
    cprintf("   F1      - Help Screen \r\n");
    cprintf("\r\n");
    cprintf("   Press Any Key for Draw ");
    getch();



}


void CustomGrid(void)
{

	int dn,ht,st,b,c,offset,step;
	int calcx,calcy,dif,a;
	step=0;

	gotoxy (1,23);
	printf("Width of Grid ");

	scanf("%d",&dn);

	if (dn <0 || dn >32)
	{
		gotoxy (1,25);
		printf("Grid Cannot Exceed 32 blocks");
	}
	else
		{
		gotoxy (1,23);
		printf("Height of Grid  ");
		scanf("%d",&ht);

		if (ht <0 || ht >32)
			{
			gotoxy (1,25);
			printf("Height Cannot Exceed 32 blocks ");
			}

	else
	{
	gotoxy (1,25);
	printf("                        ");

	// **********************************************************
	calcx = dn;
	calcy = ht;

	offset=0;
	st = 64 * 320+10;
	dif = st;

	for (a=0;a<ht*dn;a++)
	{
	   for (b=0;b<ht;b++)
	   {
		for (b=0;b<5;b++)             // 60 Lines high and 60 accross
		{
		*(screen+st+b+offset)=15;
		}
		offset = offset + 320*5;

	  }
	st = dif +5;
	offset=0;
	}
	st=64 * 320+10;                 //start location

	getch();


	  for (c=0;c<dn*ht;c++)
	  {

		for (b=0;b<5;b++)             // 60 Lines high and 60 accross
		{
		*(screen+b+st+offset+step)=15;
		offset = offset + 319;
		}
		offset = 0;
		step = step + 5;
	  }

	offset=0;
	step=0;
	}
	}

}
