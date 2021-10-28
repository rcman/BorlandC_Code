#include "tankdefs.h"
#include "types.h"
#include "extern.h"
#include <dos.h>

/* tankhit[#] Routine called when a shot hits a tank
 * void TankHit[TankNumber]
 * requires: tn is the number of the tank that was hit,
 * 			 sn is the number of the tank that gets the credit
 * ensures:  tank expolsion is shown,  opponent's score updated,
 * 			 new round started
 */
void TankHit(int tn, int sn)
{
	int i;
	tankRec *t=&tank[tn];

	drawit(t->x,t->y,TXDIM,TYDIM,TankEraseBM);
	drawit(t->x,t->y,EXP1XDIM,EXP1YDIM,Explosion1BM);
	newscreen=1;
	delay(3000);
	drawit(t->x, t->y,EXP1XDIM, EXP1YDIM, Explosion1EraseBM);
	t=&tank[sn];
   if (tn != sn) {
	   if (TreesOn) t->score+=t->treeshit + 10; 
      else t->score+=10;
   } else { 
         if (t->score>10) t->score-=10;  
   }       
	drawit(t->x, t->y,TXDIM, TYDIM, TankEraseBM);

}