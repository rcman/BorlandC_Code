#include <dos.h>

/**************************************************************************/
void SetCursor(char start, char end)
/* requires: none
 * ensures:  cursor start, end lines set as given
 *
 * Note: This routine is only used for aesthetic purposes
 */
{
	struct REGPACK regs;
	regs.r_ax=0x0100;
	regs.r_cx=(start*256)+end;
	intr(0x10, &regs);
}