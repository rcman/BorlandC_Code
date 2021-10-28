/* SHOTS contains routines to create/move tank shots */

#include <stdlib.h>
#include "stk.h"
#include "tankdefs.h"
#include "types.h"
#include "extern.h"

void ShotHit(int i, int color)
/* requires: i = number of a shot (and tank) that hit something of color
 * ensures:  shot hit handled correctly
 */
{	int treenum, os, sn;
	char temp[30];

	switch (color) {
		case TREECOLOR : /* Hit a tree */
			  treenum=field[HitX][HitY];
			  drawit(tree[treenum].x,tree[treenum].y,
			         TREEXDIM,TREEYDIM,TreeEraseBM);
			  tank[i].treeshit++;
			  break;
		case T0COLOR : TankHit(0,i); break;
		case T1COLOR : TankHit(1,i); break;
		case T2COLOR : TankHit(2,i); break;
		case T3COLOR : TankHit(3,i); break;
		case SHOTCOLOR : /* hit a shot */
			for (sn=0; sn<NumTanks; sn++)
					if ((sn!=i)&&
						 (abs(shotdata[sn].x-shotdata[i].x)<=SXDIM+1)&&
						 (abs(shotdata[sn].y-shotdata[i].y)<=SYDIM+1)) {
						  os=sn;
						  continue;
					 }
			drawit(shotdata[os].x,shotdata[os].y,SXDIM,SYDIM,ShotEraseBM);
			shotdata[os].range=0;
			break;
		}
}


void shots()
{

  static int dx[]={-1,1,0,0};
  static int dy[]={0,0,-1,1};
  int color,i;

  shotrec *s;
  tankRec *t;

  for (i=0;i<NumTanks;i++) {
  s=&shotdata[i];
  t=&tank[i];
	/* check range */
	if (s->range) {
		s->speed--;
		if (!s->speed) {
			s->speed=ShotSpeed;
			s->range--;
			if (s->range) {
				if (GuidedShots) {
					s->dir = t->dir;
					s->dx  = dx[s->dir];
					s->dy  = dy[s->dir];
				}
  				drawit(s->x,s->y,SXDIM,SYDIM,ShotEraseBM);
				if (color=hit(s->x+s->dx,s->y+s->dy,SXDIM,SYDIM,s->dir)) {
					s->range=0;
					ShotHit(i, color);
				}
				else {  /* no hit */
					s->x+=s->dx;
					s->y+=s->dy;
					drawit(s->x,s->y,SXDIM,SYDIM,ShotBM);
				}
			}
			else {
				/* it's dead */
				drawit(s->x,s->y,SXDIM,SYDIM,ShotEraseBM);
			}
		}
	}
	if (gr_keys[t->firek] && !s->range) {
		s=&shotdata[i];
		s->speed=ShotSpeed;
		switch (t->dir) {
				case UP: s->x = t->x + TXDIM /2;
							s->y = t->y - SYDIM;
							s->dx=0;
							s->dy=-1;
							s->dir = UP;
                     if ( (color=hit(s->x, s->y, SXDIM, SYDIM, UP))||
								  (color=hit(s->x, s->y+1, SXDIM, SYDIM, UP))) {
								ShotHit(i, color);
							}
                     else  s->range=ShotRange;
							break;
				case DOWN: 	s->x = t->x + TXDIM/2;
								s->y = t->y + TYDIM;
                     	if ((color=hit(s->x, s->y, SXDIM, SYDIM, DOWN)) ||
									 (color=hit(s->x, s->y-1, SXDIM,SYDIM,DOWN))) {
									ShotHit(i, color);
								}
                     	else  s->range=ShotRange;
								s->dx=0;
								s->dy=1;
								s->dir = DOWN;
								break;
				case LEFT: 	s->x = t->x - SYDIM;
								s->y = t->y +(TYDIM-1)/2;
                     	if (color=hit(s->x, s->y, SXDIM, SYDIM, LEFT)) {
									ShotHit(i, color);
								}
                     	else  s->range=ShotRange;
								s->dx=-1;
								s->dy=0;
								s->dir = LEFT;
								break;
				case RIGHT: s->x = t->x+TXDIM;
								s->y = t->y + (TYDIM-1)/2;
                     	if (color=hit(s->x, s->y, SXDIM, SYDIM, RIGHT)) {
									ShotHit(i, color);
								}
                     	else  s->range=ShotRange;
								s->dx=1;
								s->dy=0;
								s->dir = RIGHT;
								break;
			} /* SWITCH */
	  	}


  } /* for */
}