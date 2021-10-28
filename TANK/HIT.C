/* HIT contains the collision detection routines */

#include <conio.h>
#include "tankdefs.h"
#include "types.h"
#include "extern.h"

int hit( int x, int y, int xdim, int ydim, int dir)
/*	requires:	x,y coords of upper left corner object
 * 				xdim,ydim object dimensions
 * 				dir = direction to check
 * ensures: 	global vars HitX,HitY are the coordinates
 *             of the pixel that caused the hit
 * returns:	   color of first non-zero element in approp direction
 * notes:		assumes background color is 0, video mode 13         */
{
 long	int ofs,i;

	if (dir&2) {
		if (dir==UP) ofs=(HitY=(y-1))*320+x;     /* UP    */
		else         ofs=(HitY=(y+ydim))*320+x;  /* DOWN  */
		for (i=ofs;i<ofs+xdim;i++) {
			if (*(vbuf + i)) { HitX=i-ofs+x; return (int) *(vbuf + i);}
		}
		return 0; /* no collision */
	}
	else {
		if (dir==LEFT) ofs=y*320+(HitX=x-1);		/* LEFT */
	   else ofs=y*320+(HitX=x+xdim);					/* RIGHT */
		for (i=0;i<ydim;i++) {
			if (*(vbuf +ofs)) { HitY=i+y; return (int) *(vbuf+ofs); }
			ofs+=320;
		}
		return 0; /* no collision */
	}
}