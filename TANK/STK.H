/**********************************************************************
* stk.h
* 
* All prototypes and global typedefs for the sprite toolkit
* concatenated into one include file. (The separate include 
* files can be found from the stksrc archieve.)
*
* The files marked with S are selfcontained, and may be used
* alone. The rest of the files need the definitions from the 
* file spr.h. 
*
* "grtypes.h"    ** Misc type defs                      (S) **
* "gr.h"         ** Graphics start, end, text I/O       (S) **
* "mouse.h"      ** Mouse (INT 33) interface            (S) **
* "spr.h"        ** Sprite init, create, display, erase (S) **
* "spr_hit.h"    ** Sprite collision detection              **
* "spr_fio.h"    ** Sprite file I/O                         **
* "spr_anim.h"   ** Automatic spr animation & movement      **
**********************************************************************
                    This file is part of

          STK -- The sprite toolkit -- version 1.0

              Copyright (C) Jari Karjala 1990

The sprite toolkit (STK) is a FreeWare toolkit for creating high
resolution sprite graphics with PCompatible hardware. This toolkit 
is provided as is without any warranty or such thing. See the file
COPYING for further information.

**********************************************************************
**********************************************************************/


#if     !defined(__GRTYPES_H_)
#define __GRTYPES_H_
#ifndef NULL
#define NULL ((void*)0)
#endif
typedef unsigned char BYTE;
typedef unsigned int  WORD;
typedef BYTE *BITMAP;
#endif
#if     !defined(__GR_H_)
#define __GR_H_
#define GR_TYPE_ANY 0   /* Any mode will do */
#define GR_TYPE_SPR 1   /* The best possible mode for the sprite toolkit */
void gr_detect(int type, int *graphdriver, int *graphmode);
void gr_start(int *graphdriver, int *graphmode);
void gr_end(void);
void gr_putch(char ch);
void gr_puts(char *s);
void gr_printf(char *s,...);
#define gr_gotoxy(x, y) moveto(x*8, y*8)
void gr_xy_printf(int x, int y, char *s,...);
void gr_dual_xy_printf(int x, int y, char *s,...);
int gr_inkey(void);
char *gr_gets(char *cpdest, int max_len);
extern int gr_max_x;
extern int gr_max_y;
extern int gr_text_mode;
#define GR_MODE_OR      (1<<0)      /* OR the text over previous graphics */
#define GR_MODE_CLEAR   (1<<1)      /* Clear the backgroud before print  */
extern char gr_keys[128];
void gr_start_kbd_grab(void);
void gr_end_kbd_grab(void);
#define GR_KEY_ESC  1
#define GR_KEY_1    2
#define GR_KEY_2    3
#define GR_KEY_3    4
#define GR_KEY_4    5
#define GR_KEY_5    6
#define GR_KEY_6    7
#define GR_KEY_7    8
#define GR_KEY_8    9
#define GR_KEY_9    10
#define GR_KEY_0    11
#define GR_KEY_TAB  15
#define GR_KEY_Q    16
#define GR_KEY_W    17
#define GR_KEY_E    18
#define GR_KEY_R    19 
#define GR_KEY_T    20
#define GR_KEY_Y    21
#define GR_KEY_U    22
#define GR_KEY_I    23
#define GR_KEY_O    24
#define GR_KEY_P    25
#define GR_KEY_A    30
#define GR_KEY_S    31
#define GR_KEY_D    32
#define GR_KEY_F    33
#define GR_KEY_G    34
#define GR_KEY_H    35
#define GR_KEY_J    36
#define GR_KEY_K    37
#define GR_KEY_L    38
#define GR_KEY_Z    44
#define GR_KEY_X    45 
#define GR_KEY_C    46
#define GR_KEY_V    47
#define GR_KEY_B    48
#define GR_KEY_N    49
#define GR_KEY_M    50
#define GR_KEY_COMMA    51
#define GR_KEY_DOT      52
#define GR_KEY_SPACE    57
#define GR_KEY_ARROW_UP     72
#define GR_KEY_ARROW_DOWN   80
#define GR_KEY_ARROW_LEFT   75
#define GR_KEY_ARROW_RIGHT  77
#endif
#if     !defined(__MOUSE_H_)
#define __MOUSE_H_
int     mouse_initialize(void);
#endif
#ifndef __SPR_H_
#define __SPR_H_
extern int spr_pass_delay;
#define spr_max_x gr_max_x
#define spr_max_y gr_max_y
#ifndef __SPRITE_
typedef void *SPRITE;
#endif
void spr_initialize(int graphicsdriver);
SPRITE spr_create(WORD w, WORD h, 
                  BITMAP pic, BITMAP mask, 
                  BYTE res, WORD ID);
SPRITE spr_share(SPRITE spr, BYTE n);
SPRITE spr_copy(SPRITE spr, WORD id);
void spr_put(SPRITE spr, WORD x, WORD y);
void spr_hide(SPRITE spr);
void spr_delete(SPRITE spr);
WORD spr_next_pass(void);
void spr_regulate_speed(void);
WORD spr_get_id(SPRITE spr);
WORD spr_get_x(SPRITE spr);
WORD spr_get_y(SPRITE spr);
WORD spr_get_width(SPRITE spr);
WORD spr_get_height(SPRITE spr);
#endif
#ifndef __SPR_HIT_H_
#define __SPR_HIT_H_
int spr_hit_with_point(SPRITE spr, WORD x, WORD y);
int spr_hit(SPRITE spr1, SPRITE spr2);
SPRITE spr_hit_first(SPRITE spr);
SPRITE spr_hit_next(SPRITE spr);
#endif
#ifndef __SPR_FIO_H_
#define __SPR_FIO_H_
SPRITE spr_fio_read_smp(char *smpfile, BYTE res, WORD ID);
#endif
#ifndef __SPR_ANIM_H_
#define __SPR_ANIM_H_
#ifndef __ANIM_SPRITE_
typedef void *ANIM_SPRITE;
#endif
typedef struct _anim_spr_info {
    int     x,y;                            /** location        **/
    int     dx,dy;                          /** movement vector **/
    int     lef, top, rig, bot;             /** limits          **/
    WORD    frame, frame_delay, timeout;    /** time info       **/
    WORD    id;     /** the user spesified id of current sprite **/
    int     w,h;                            /** width&height    **/
} ANIM_SPR_INFO;
#define SPR_ANIM_FX_ALL       (0xFFFF) /** all fx       **/
#define SPR_ANIM_FX_TIMEOUT     (1<<0) /** timeout      **/
#define SPR_ANIM_FX_HIT_X_LIMIT (1<<1) /** hit x limit  **/
#define SPR_ANIM_FX_HIT_Y_LIMIT (1<<2) /** hit y limit  **/
#define SPR_ANIM_FX_HIT_SPRITE  (1<<3) /** hit other spr**/
#define SPR_ANIM_FX_RET_NOTHING (0)     /** continue normally       **/
#define SPR_ANIM_FX_RET_RE_PUT  (1)     /** put the sprite again    **/
#define SPR_ANIM_FX_RET_STOP    (2)     /** stop the animation      **/
#define SPR_ANIM_FX_RET_DELETE  (3)     /** delete the anim.sprite  **/
#define SPR_ANIM_FX_RET_DESTROY (4)     /** destroy the anim.sprite **/
ANIM_SPRITE spr_anim_create(WORD count, ...);
void spr_anim_start(ANIM_SPRITE aspr);
void spr_anim_stop(ANIM_SPRITE aspr);
void spr_anim_delete(ANIM_SPRITE aspr);
void spr_anim_destroy(ANIM_SPRITE aspr);
WORD spr_anim_next_pass(void);
ANIM_SPR_INFO *spr_anim_get_info(ANIM_SPRITE aspr);
void spr_anim_set_location(ANIM_SPRITE aspr, WORD x, WORD y);
void spr_anim_set_vector(ANIM_SPRITE aspr, int dx, int dy);
void spr_anim_set_limits(ANIM_SPRITE aspr,
                         WORD lef, WORD top, WORD rig, WORD bot);
void spr_anim_set_time(ANIM_SPRITE aspr,
                       int frame, int frame_delay, int timeout);
void spr_anim_set_fx_handler(ANIM_SPRITE aspr, 
                             WORD fx_mask, 
                             WORD (fx_handler)(ANIM_SPRITE,WORD,SPRITE));
#endif
