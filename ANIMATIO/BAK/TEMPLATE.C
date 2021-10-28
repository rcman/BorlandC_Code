/*
			TEMPLATE.C : Used by my demomstration program KEYS.C using
			my KEYBOARD.C lib.
			Copyright (c) 1995 by Mark C. Williston.
			EMAIL: mwillist@fox.nstn.ca
			Released to the PUBLIC DOMAIN which can be used in any way
			or place you see fit.
			I hope it help anyone! Mail me if it does.
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <mem.h>

extern unsigned char KeyTable[];
extern unsigned char ScanCode;
extern unsigned char KeyboardStatus;

#include "scantab.inc"

void BuildTemplate(void)
{
	FILE *in;
	char far * Buffer;
	char far * TextScreen = (char far *) 0xB8000000L;

	in=fopen("KEYBOARD.BIN","rb");
	if(in==NULL)
	{
		printf("ERROR: Keyboard template (KEYBOARD.BIN) was not loaded\n");
		exit(1);
	}
	if ((Buffer = (char far *) malloc(4000)) == NULL)
	{
		printf("Not enough memory to allocate buffer\n");
		exit(1);
	}
	fseek(in, SEEK_SET, 0);
	fread(Buffer, 4000, 1, in);
	memcpy(TextScreen, Buffer, 4000);
	free(Buffer);
	fclose(in);
}



void FlipLites(int Led)
{

	Lites[Led].On^=1;
	gotoxy(Lites[Led].X, Lites[Led].Y);

	if(Lites[Led].On)
		textcolor(12);
	else
		textcolor(7);
	cprintf("%c",LIGHT);
}