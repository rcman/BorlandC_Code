#include "tankdefs.h"
#include "types.h"
#include "extern.h"

void PutString13(char *buf, int x, int y, int f, int b)
/* requires: in graphics mode 13, buf contains a null terminated string
 * ensures:  string buf written to screen starting at x,y
 * 	       with characters 8 spaces apart and colors f/b
 */
{
	int i;
	for (i=0;buf[i];i++) {
		DisplayChar13(*(buf+i), x, y, f, b);
		x += 8;
	}
}