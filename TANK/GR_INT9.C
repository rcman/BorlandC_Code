/**********************************************************************
* gr_int9.c
*
* New keyboard interrupt service routine.
* MUST BE COMPILED WITHOUT STACK CHECKS AND REGISTER VARIABLES
**********************************************************************
                    This file is part of

         STK -- The sprite toolkit -- version 1.0

              Copyright (C) Jari Karjala 1990

The sprite toolkit (STK) is a FreeWare toolkit for creating high
resolution sprite graphics with PCompatible hardware. This toolkit 
is provided as is without any warranty or such thing. See the file
COPYING for further information.

**********************************************************************/

#include <dos.h>

#include "gr.h"


/** Vector to the old keyboard interrupt handler **/
void interrupt (*gr_old_int9)(void);

static unsigned int far *fp_kbd_tail, far *fp_kbd_head;

/** The new keyboard interrupt handler, see start_kbd_grab() in gr.c **/
void interrupt gr_int9(void)
{
    unsigned char a;

    a = inp(0x60);              /** read key code and (re)set it to table **/
    if (a & 0x80)
        gr_keys[a & 0x7F] = 0;
    else
        gr_keys[a] = 1;

    fp_kbd_tail = MK_FP(0x40,0x1A);
    fp_kbd_head = MK_FP(0x40,0x1C);
    *fp_kbd_tail = *fp_kbd_head;          /** flush keyboard buffer **/

    gr_old_int9();                 /** allow old handler (eg breakon.com) **/
}
