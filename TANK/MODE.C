#include <dos.h>

/**************************************************************************/
void mode(int VideoMode)
/* requires: none
 * ensures:  sets video mode to VideoMode
 */
{
	struct REGPACK regs;
	regs.r_ax=VideoMode;
	intr(0x10, &regs);
}