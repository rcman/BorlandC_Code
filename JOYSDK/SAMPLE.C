/*
Sample C code for reading joystick axis

Joysticks & gamecards vary in their physical characteristics from one to
another.  For this reason the high state duration of joysticks in their extreme
positions must be predetermined for each joystick & gamecard combinations.
The following program will prompt the user to move the joystick to all its
extreme positions at startup time.  Axis high state duration will be recorded
at these positions for computing joystick position at a later time in the
programs.
*/

#include <conio.h>
#include <stdio.h>

/*
Declare external functions
*/
extern void get_loop_count (void);  /* Assembly language module initialization function*/
extern short read_axis (short);  /* Return axis position count function */

/*
Declare local functions
*/
short compute_axis (short, short, short);  /* Compute axis coordinate */
void display_coordinate (void);  /* Display joystick coordinate */

/*
Declare variable
*/
short stick_a_min_x;  /* Minimum high state duration for X axis of joystick A */
short stick_a_max_x;  /* Maximun high state duration for X axis of joystick A */
short stick_a_range_x;	/* Joystick A - X axis travel range */
short stick_a_min_y;  /* Minimum high state duration for Y axis of joystick A */
short stick_a_max_y;  /* Maximum high state duration for Y axis of joystick A */
short stick_a_range_y;	/* Joystick A - Y axis travel range */
short stick_b_min_x;  /* Minimum high state duration for X axis of joystick B */
short stick_b_max_x;  /* Maximun high state duration for X axis of joystick B */
short stick_b_range_x;	/* Joystick B - X axis travel range */
short stick_b_min_y;  /* Minimum high state duration for Y axis of joystick B */
short stick_b_max_y;  /* Maximum high state duration for Y axis of joystick B */
short stick_b_range_y;	/* Joystick B - Y axis travel range */

main ()
{
	/* Initialize assembly language module by calling get_loop_count */
	get_loop_count ();

	/* Joystick A */
	printf ("Push spacebar when joystick A is farthest to left\n");
	/* Wait for spacebar pressed */
	while (getch () != 0x20)
		;
	/* Spacebar pressed, read joystick A - X axis */
	stick_a_min_x = read_axis (0);

	printf ("Push spacebar when joystick A is farthest to right\n");
	/* Wait for spacebar pressed */
	while (getch () != 0x20)
		;
	/* Spacebar pressed, read joystick A - X axis */
	stick_a_max_x = read_axis (0);

	/* Compute joystick A - X axis travel range */
	stick_a_range_x = stick_a_max_x - stick_a_min_x;


	printf ("Push spacebar when joystick A is farthest to top\n");
	/* Wait for spacebar pressed */
	while (getch () != 0x20)
		;
	/* Spacebar pressed, read joystick A - Y axis */
	stick_a_min_y = read_axis (1);

	printf ("Push spacebar when joystick A is farthest to bottom\n");
	/* Wait for spacebar pressed */
	while (getch () != 0x20)
		;
	/* Spacebar pressed, read joystick A - Y axis */
	stick_a_max_y = read_axis (1);

	/* Compute joystick A - Y axis travel range */
	stick_a_range_y = stick_a_max_y - stick_a_min_y;



	/* Joystick B */
	printf ("Push spacebar when joystick B is farthest to left\n");
	/* Wait for spacebar pressed */
	while (getch () != 0x20)
		;
	/* Spacebar pressed, read joystick B - X axis */
	stick_b_min_x = read_axis (2);

	printf ("Push spacebar when joystick B is farthest to right\n");
	/* Wait for spacebar pressed */
	while (getch () != 0x20)
		;
	/* Spacebar pressed, read joystick B - X axis */
	stick_b_max_x = read_axis (2);

	/* Compute joystick B - X axis travel range */
	stick_b_range_x = stick_b_max_x - stick_b_min_x;


	printf ("Push spacebar when joystick B is farthest to top\n");
	/* Wait for spacebar pressed */
	while (getch () != 0x20)
		;
	/* Spacebar pressed, read joystick B - Y axis */
	stick_b_min_y = read_axis (3);

	printf ("Push spacebar when joystick B is farthest to bottom\n");
	/* Wait for spacebar pressed */
	while (getch () != 0x20)
		;
	/* Spacebar pressed, read joystick B - Y axis */
	stick_b_max_y = read_axis (3);

	/* Compute joystick B - Y axis travel range */
	stick_b_range_y = stick_b_max_y - stick_b_min_y;



	/* Display joystick coordinate */
	while (!kbhit ())
	      display_coordinate ();
	/* Flush keyboard buffer before quitting */
	if (getch () == 0)
		getch ();
}


/*
Display all 4 joystick coordinates from a range of 0 to 100
*/

void display_coordinate ()
{
	short stick_a_x, stick_a_y, stick_b_x, stick_b_y;

	stick_a_x = read_axis (0);
	if (stick_a_x < stick_a_min_x)
		stick_a_x = stick_a_min_x;
	else
		if (stick_a_x > stick_a_max_x)
			stick_a_x = stick_a_max_x;
	if (stick_a_range_x == 0)
		printf ("         0");
	else
		printf ("%10d", compute_axis (stick_a_x, stick_a_min_x, stick_a_range_x));

	stick_a_y = read_axis (1);
	if (stick_a_y < stick_a_min_y)
		stick_a_y = stick_a_min_y;
	else
		if (stick_a_y > stick_a_max_y)
			stick_a_y = stick_a_max_y;
	if (stick_a_range_y == 0)
		printf ("         0");
	else
		printf ("%10d", compute_axis (stick_a_y, stick_a_min_y, stick_a_range_y));


	stick_b_x = read_axis (2);
	if (stick_b_x < stick_b_min_x)
		stick_b_x = stick_b_min_x;
	else
		if (stick_b_x > stick_b_max_x)
			stick_b_x = stick_b_max_x;
	if (stick_b_range_x == 0)
		printf ("         0");
	else
		printf ("%10d", compute_axis (stick_b_x, stick_b_min_x, stick_b_range_x));

	stick_b_y = read_axis (3);
	if (stick_b_y < stick_b_min_y)
		stick_b_y = stick_b_min_y;
	else
		if (stick_b_y > stick_b_max_y)
			stick_b_y = stick_b_max_y;
	if (stick_b_range_y == 0)
		printf ("         0\n");
	else
		printf ("%10d\n", compute_axis (stick_b_y, stick_b_min_y, stick_b_range_y));
}


/*
Compute axis coordinate from a range of 0 to 100
*/
short compute_axis (axis, min, range)
short axis, min, range;
{
	return (((long) 100 * (long) (axis - min)) / (long) range);
}
