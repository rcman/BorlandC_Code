;Caller: int ReadPixel13(x,y);
;            int x,y
;

ARGx		EQU		word ptr [bp+4]
ARGy		EQU		word ptr [bp+6]

_TEXT		SEGMENT	byte public 'CODE'
		ASSUME	cs:_TEXT

		EXTRN	PixelAddr13:near
		PUBLIC	_ReadPixel13
_ReadPixel13	PROC	near

		push	bp
		mov	bp,sp

		mov	ax,ARGy
		mov	bx,ARGx
		call	PixelAddr13

		mov	al,es:[bx]
		xor	ah,ah

		mov	sp,bp
		pop	bp
		ret

_ReadPixel13	ENDP

_TEXT		ENDS
		END