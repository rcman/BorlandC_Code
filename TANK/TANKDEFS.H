/* TANKDEFS.H
 *
 * This file contains all global defines for the Tank program.
 */

#define MAXNUMTANKS 3

#define CONFIGFILE "tank.cfg" /* configuration file name */

/* tank data */
#define DEFTANKSPEED 260 /* number of main loops between each tank movement */
#define DEFNUMTANKS 2
#define T0COLOR 0x09
#define T1COLOR 0x0c
#define T2COLOR 0x0e
#define T3COLOR 0x0b

/* shot data */
#define SHOTCOLOR    0x0f
#define DEFSHOTRANGE 220  /* max number of pixels shot moves before dying out */
#define DEFSHOTSPEED 190  /* number of main loops between each shot movement  */

/* Tree Stuff */
#define TREECOLOR 0x02
#define NUMTREES 25

/* bitmap dimensions */
#define TXDIM  12              /* number of columns in a tank bitmap  */
#define TYDIM  12              /* number of rows in a tank bitmap */
#define TBMLEN  TXDIM*TYDIM    /* number of bytes in a tank pic */

#define SXDIM 2
#define SYDIM 2
#define SBMLEN SXDIM*SYDIM

#define EXP1XDIM 11
#define EXP1YDIM 11
#define EXP1BMLEN EXP1XDIM*EXP1YDIM

#define TREEXDIM 10
#define TREEYDIM 11
#define TREEBMLEN TREEXDIM*TREEYDIM

/* Dimensions of field[x][y] array. See field in EXTERN.H */
#define FIELDX 305
#define FIELDY 175

/* direction (usuallly 'dir') definitions */
#define LEFT  0
#define RIGHT 1
#define UP    2
#define DOWN  3

#define CursorOff SetCursor(32, 32)
#define CursorOn  SetCursor(6, 7)