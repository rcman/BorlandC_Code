; void SetPixel(x,y,n)
;	int x,y /* pixel corrds */
;	int n	/* color */
ARGx	EQU	word ptr [bp+4]
ARGy	EQU	word ptr [bp+6]
ARGn	EQU	byte ptr [bp+8]


_TEXT	SEGMENT	byte public 'CODE'
	ASSUME	cs:_TEXT

	EXTRN	PixelAddr13:near

	PUBLIC	_SetPixel13
_SetPixel13	PROC	near

	push	bp
	mov 	bp,sp

	mov	ax,ARGy
	mov	bx,ARGx
	call	PixelAddr13

	mov	al,ARGn
	mov	es:[bx],al

	mov	sp,bp
	pop	bp
	ret

_SetPixel13	ENDP
_TEXT	ENDS
	END