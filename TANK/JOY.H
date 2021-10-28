/* Demonstration of reading joy stick postition
	Written March 24, 1988
	Written by : ajmyrvold@violet.waterloo.edu (Alan J. Myrvold)
	Technical assistance from : uunet!netxcom!jallen (John Allen)
	Turbo C Version 1.0

	Changes: 1/93 Kevin Dahlhausen ap0962po.cwru.edu
		thresholds, JoyStickx()

*/

#ifndef __joystick__

#define __joystick__
typedef struct {
	int sw1,sw2;
	int x,y;
	int cenx,ceny;
	int tu, td, tl, tr;  /* threshholds */
} joy_stick;

#define JoyStickLeft(j)		(j.x <= j.tl)
#define JoyStickRight(j)	(j.x >= j.tr)
#define JoyStickUp(j)		(j.y <= j.tu)
#define JoyStickDown(j)		(j.y >= j.td)
#define JoyButton1(j)		(j.sw1)
#define JoyButton2(j)		(j.sw2)

extern void read_stick(int stick,joy_stick *joy);
extern int choose_stick();
extern void set_thresh(int stick, joy_stick *joy);
extern int joy_state(int stick, joy_stick *j);
#endif