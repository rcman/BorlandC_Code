; PixelAddr13: 	determines buffer address of pixel in
;		320 by 200 256 color mode
; Called:	AX=y-coordinate (0-99)
;		BX=x-coordinate (0-319)
; Returns:	BX=byte offset in buffer
;		ES=video buffer segment

OriginOffset	EQU	0
VideoBufferSeg	EQU	0A000h

_TEXT		SEGMENT	byte public 'CODE'
		ASSUME	cs:_TEXT

		PUBLIC	PixelAddr13
PixelAddr13	PROC	near

		xchg	ah,al
		add	bx,ax
		shr	ax,1
		shr	ax,1
		add	bx,ax

		add	bx,OriginOffset

		mov	ax,VideoBufferSeg
		mov	es,ax
		ret

PixelAddr13	ENDP

_TEXT		ENDS
		END