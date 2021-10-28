		page	,132
;------------------------------------------------------------------------------
; Module Name: READAXIS.ASM
;
; Assembly language routine for reading joystick axis position
;
; Created: Feb 17th, 1992
; Author: Peter Wan
;
; Copyright (c) 1992 Advanced Gravis Computer Technology Limited
;
; Public Functions:
;	get_loop_count
;	read_axis
; Public Data:
;	None
; General Description:
;	This module contains functions to initialize itself and return
;	joystick axis position.
;------------------------------------------------------------------------------

		dosseg
		.model	 small

;------------------------------------------------------------------------------
; Port address
;------------------------------------------------------------------------------
GAMEPORT	equ	201h		;gameport address
TIMER0		equ	40h		;8253 counter timer channel 0 address
TIMER_CONTROL	equ	43h		;counter timer control register address

		.data
max_loop_count	dw	?		;loop count for 3 milliseconds

		.code

;------------------------------------------------------------------------------
; get_loop_count
;
; The purpose of this routine is to determine how many iteration of the
; following instruction loop can be accomplished in 3 milliseconds.  The number
; of iteration count will be different on computers running at different speed
; or computers with different processing unit.	In the read joystick axis
; routine, it uses a similar program loop to determine joystick axis position
; and the count results from this routine will be used to limit axis read time
; up to 3 millesecond maximum.	This routine must be called once at
; initialization time.
;
; Entry:
;	None
; Returns:
;	None
; Error Returns:
;	None
; Registers Preserved:
;	BX,SI,DI,BP,DS,ES
; Registers Destroyed:
;	AX,CX,DX
; Calls:
;	None
;------------------------------------------------------------------------------

		public	_get_loop_count
_get_loop_count proc
init_count	equ	<[bp-2]>
		push	bp
		mov	bp,sp
		sub	sp,2		;2 bytes of local variable
		mov	max_loop_count,0;use loop count of 16 to start with
		mov	dx,GAMEPORT	;gameport address
glc3:
		mov	ax,0ff00h
		add	max_loop_count,16
		mov	cx,max_loop_count
		cli			;system interrupt must be disabled
		out	TIMER_CONTROL,al;latch count in timer 0
		jmp	short $+2
		in	al,TIMER0	;read low byte of latched count
		mov	init_count,al	;save
		jmp	short $+2
		in	al,TIMER0	;read high byte
		mov	init_count+1,al ;save
		out	dx,al		;trigger gameport
		in	al,dx		;delay to allow gameport to switch on
glc4:					;the following loop exits only when CX
					;reaches zero
		in	al,dx
		or	al,ah
		loopnz	glc4
		mov	ax,0
		jnz	$+2
		out	TIMER_CONTROL,al;latch count in timer 0
		sti			;enable system interrupt
		jmp	short $+2
		in	al,TIMER0	;read low byte of latched count
		mov	ah,al
		jmp	short $+2
		in	al,TIMER0	;read high byte
		xchg	al,ah
		sub	ax,init_count	;subtract count latched before
					;entering loop by count just read
		neg	ax
		cmp	ax,7160 	;has it exceed 3 milliseconds?
		jl	glc3		;no, increment loop count by 16 and
					;repeat process
		mov	sp,bp
		pop	bp
		ret
_get_loop_count endp

;------------------------------------------------------------------------------
; _read_axis
;
; This routine will accept a number from 0 to 3 for returning joystick axis
; position for joystick A-X, A-Y, B-X, B-Y respectively.  After bit position
; of requested axis is determined, count reading in counter timer chip channel 0
; will be read and saved.  A write cycle to gameport address 201 hex will
; trigger and toggle all four axis lines from low to high state.  The duration
; of these individual line staying high is directly proportional to the
; position of joystick for the individual axis.  After the requested axis line
; has returned to the low state, count reading in counter timer chip will be
; read again. The real time count of the duration the requested axis has stayed
; high can be obtained from the difference of the two counts read.  Allowance
; for other axis to return to low state must also be provided before next read.
; Therefore, return to calling function can only be done at the end of the
; 3 milliseconds duration.  The real time count return to the callind function
; can have a value ranges from 1 to maximum of 7160 depending on the position
; of the joystick, value of capacitors used and threshold voltage set
; on the gamecard.  If the requested axis has not returned to the low state
; after 3 milliseconds, it is assumed that no joystick is connected to that
; axis and zero will be returned to the calling function.  During the
; 3 milliseconds duration, all interrupts except NMI will be disabled.
;
; Entry:
;	[bp+4] = Axis to be read
;		 0 = Joystick A - X axis
;		 1 = Joystick A - Y axis
;		 2 = Joystick B - X axis
;		 3 = Joystick B - Y axis
; Returns:
;	AX = Count ranges from 1 to maximun of 7160 corresponds to joystick
;	     position
; Error Returns:
;	AX = 0 if no joystick connected
; Registers Preserved:
;	BX,SI,DI,BP,DS,ES
; Registers Destroyed:
;	AX,CX,DX
; Calls:
;	None
;------------------------------------------------------------------------------

		public	_read_axis
_read_axis	proc
axis		equ	<[bp+4]>
init_count	equ	<[bp-2]>
		push	bp
		mov	bp,sp
		sub	sp,2		;2 bytes of local variable
		sub	ax,ax		;determine bit position of axis to be
		mov	ah,1		;read
		mov	cx,axis
		shl	ah,cl
		mov	cx,max_loop_count;use 3 milliseconds loop count obtian
					;at initailization
		mov	dx,GAMEPORT	;gameport address
		cli			;disable interrupt
		out	TIMER_CONTROL,al;latch count in timer 0
		jmp	short $+2
		in	al,TIMER0	;read low byte of latched count
		mov	init_count,al	;save
		jmp	short $+2
		in	al,TIMER0	;read high byte
		mov	init_count+1,al ;save
		out	dx,al		;trigger gameport
		in	al,dx		;delay to allow gameport to switch on
ra1:
		in	al,dx		;read gameport
		test	al,ah		;requested axis return to low state or
					;3 milliseconds passed?
		loopnz	ra1		;no
		mov	ax,0		;return 0 if 3 milliseconds passed and
		jnz	ra3		;requested axis not yet in low state
		out	TIMER_CONTROL,al;requested axis return to low state
					;latch count in timer 0
		jmp	short $+2
		in	al,TIMER0	;read low byte of latched count
		mov	ah,al
		jmp	short $+2
		in	al,TIMER0	;read high byte
		xchg	al,ah
		sub	ax,init_count	;obtain real time count of high state
		neg	ax		;duration
		jcxz	ra3		;requested axis return to low state in
					;exactly 3 milliseconds, all other axis
					;should also have returned to low state
					;by now
		push	ax		;even though the requested axis has
		mov	ah,0ffh 	;returned to the low state, other axis
ra2:					;may still be in the high state, we
		in	al,dx		;should wait till the end of the
		or	al,ah		;3 milliseconds duration to allow the
		loopnz	ra2		;other axis to return to high state
		pop	ax		;before returning to the calling
					;fucntion
ra3:
		sti			;enable system interrupt
		mov	sp,bp
		pop	bp
		ret
_read_axis	endp
		end
