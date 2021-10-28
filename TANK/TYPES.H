/* TYPES.H  Type definitions for the tank game */

typedef struct {
	char 	 pic[4][TBMLEN]; 	   /* char array of color codes  */
	int 		dir;					/* direction tank is facing   */
	int		x;						/* position                   */
	int		y;
	int		xmove;				/* number pixels to move in x */
	int		ymove;   			/* number pixels to move in y */
	int      score;
	int	   treeshit;         /* number of trees hit during round */
	int		speed;            /* see TANKSPEED in defs.h          */
	char	upk;                 /* scan codes to move this tank     */
	char	downk;
	char	leftk;
	char	rightk;
	char	firek;
} tankRec;

typedef struct shotrec_s {
	int	x;
	int	y;
	int	dx;
	int	dy;
	int	range;
	int	dir;
	int	speed;
	int	owner;
} shotrec;

typedef struct {
	int	x;
	int	y;
} treeRec;