/* Demonstration of reading joy stick postition
	Written March 24, 1988
	Written by : ajmyrvold@violet.waterloo.edu (Alan J. Myrvold)
	Technical assistance from : uunet!netxcom!jallen (John Allen)
	Turbo C Version 1.0

	Changes: 1/93 Kevin Dahlhausen ap0962po.cwru.edu
		thresholds, JoyStickx()

*/

#include <stdio.h>
#include <dos.h>
#include <bios.h>
#include "joy.h"

/*
From: uunet!netxcom!jallen (John Allen)

1.  Trigger the joystick oneshots with an 'out' to 0x201.
	This will set all of the joystick bits on.

2.  Read (in) 0x201, finding:

	Bit		Contents
	0		Joystick A X coordinate
	1		Joystick A Y coordinate
	2		Joystick B X coordinate
	3		Joystick B Y coordinate
	4		Button A 1
	5		Button A 2
	6		Button B 1
	7		Button B 2

3.  Continue reading 0x201 until all oneshots return to zero,
    recording the loop during which each bit falls to zero.
	The duration of the pulse from each oneshot may be used to
    determine the resistive load (from 0 to 100K) from each
	Joystick, as: Time = 24.2msec. + .011 (r) msec.

4.  To do this correctly, I recommend calibrating the joystick;
	have the user move the stick to each corner, then center it,
	while recording the resulting values.
*/

void read_stick(int stick,joy_stick *joy)
{
	int k,jx,jy,i;
	int c,m1,m2,m3,m4,m5;

/* Define masks for the chosen joystick */
	if (stick == 1) m4 = 1; else
	if (stick == 2) m4 = 4; else
	printf("Invalid stick %d\n",stick);

	m5 = m4 << 1;
	m1 = m4 << 4;
	m2 = m5 << 4;
	m3 = m4 + m5;

/* Trigger joystick */
	outportb(0x201,0xff);
	c = inportb(0x201);

/* Read switch settings */
	joy -> sw1 = (c & m1) == 0;
    joy -> sw2 = (c & m2) == 0;

/* Get X and Y positions */
	for (k = 0; (c & m3) != 0; k++) {
	if ((c & m4) != 0) jx = k;
	else i=k;
	if ((c & m5) != 0) jy = k;
	else i=k;
	c = inportb(0x201);
    }
	
	joy -> x = jx; // - (joy -> cenx);
	joy -> y = jy; // - (joy -> ceny);

	for (jx=k; jx<300; jx++) {
		if (k || i) jy=k;
		if (k || i) jy=k;
		c=inportb(0x201);
	}
}

int choose_stick()
{
	int init_swa,init_swb,swa,swb;
	int c,retval;

	printf("Press fire.\n");
	outportb(0x201,0xff);
	c = inportb(0x201);
	init_swa = c & 0x30;
	init_swb = c & 0xc0;
	do {
	   outportb(0x201,0xff);
	   c = inportb(0x201);
	   swa = c & 0x30;
	   swb = c & 0xc0;
	} while ((swa == init_swa) && (swb == init_swb));
	if (swa != init_swa) {
	   printf("Joystick 1 selected\n");
	   retval = 1;
	} else if (swb != init_swb) {
	   printf("Joystick 2 selected\n");
	   retval = 2;
	} else {
	   printf("Keyboard selected\n");
	   retval = 0;
	}

	return(retval);
}

void set_thresh(int stick, joy_stick *joy)
/* requires: joy initialized
 * ensures:  thresholds set
 */
{
 joy->cenx=joy->ceny=0;
 while(joy->sw1||joy->sw2) read_stick(stick, joy);
 printf("\nCenter joystick and press one of the buttons.");
 do {
	read_stick(stick, joy);
 } while (!(joy->sw1) && !(joy->sw2));
 joy->ceny=joy->y;
 joy->cenx=joy->x;


 while(joy->sw1||joy->sw2) read_stick(stick, joy);
 printf("\nMove joystick to upper left and press one of the buttons.");
 do {
	read_stick(stick, joy);
 } while (!(joy->sw1) && !(joy->sw2));
 joy->tu=joy->ceny-((joy->ceny-joy->y)/2);
 joy->tl=joy->cenx-((joy->cenx-joy->x)/2);

 while(joy->sw1||joy->sw2) read_stick(stick, joy);
 printf("\nMove joystick to lower right and press one of the buttons.");
 do {
	read_stick(stick, joy);
 } while (!(joy->sw1) && !(joy->sw2));
 joy->td=joy->ceny+((joy->y-joy->ceny)/2);
 joy->tr=joy->cenx+((joy->x-joy->cenx)/2);
}

int joy_state(int stick, joy_stick *j)
/* bitmap:    xxx4321 ->  4=left, 2=right, 3=up 1=down */
{
	int rval=0;
	read_stick(stick, j);
	if (j->x <= j->tl) rval=8;
	if (j->y <= j->tu) rval=rval+4;
	if (j->x >= j->tr) rval=rval+2;
	if (j->y >= j->td) rval=rval+1;
	return rval;
}