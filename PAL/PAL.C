

#include <stdio.h>
#include <dos.h>

unsigned char inportb (unsigned portid);
void savepal(void);

unsigned char pal[768];



void main(void)
{

	savepal();


}

void savepal(void)
{
	int i;
	outp(0x3c9,0);

	for (i=0;i<256*3;i++)
	{
	*(pal+i) = inp(0x3c7);

	}
}

void FadeOut(void)
{
	int i,j;
	char *p;


	for (i=0;i<64;i++)
	{
		for(j=0;j<768;j++)
		{
		p = pal;
		if (p!=0)
		{
			p++;
		}
		}
	}





}


/*

FadeOut PROC NEAR            ; Fades the screen to black
    cli                      ; Disable interrupts
    lea     bp,Parray        ; Load offset Palette array
    mov     cx,64            ; Set loopcounter
Loop64:
    xor     bx,bx            ; Set bx to 0
Loop768:
    cmp     byte ptr [bp],0  ; Is it 0 already?
    je      FadeOn
    dec     byte ptr [bp]    ; Decrease the value with 1
FadeOn:
    inc     bp               ; Point to next cel in array
    inc     bx               ; Increase loopcounter
    cmp     bx,768           ; Done all registers?
    jl      Loop768          ; If not, loop again
    push    cx               ; Save first loopcounter
    mov     cx,768           ; Write all new R,G,B values
    call    WaitVrt          ; Wait for a vertical retrace
    call    WaitVrt          ; Twice
    sub     bp,768           ; Reset bp to 0
    mov     dx,03c8h         ; Write register
    mov     al,0             ; Start writing at color 0
    out     dx,al            ; Give info to VGA
    mov     dx,03c9h         ; Data register
WriteAll:
    mov     al,byte ptr [bp] ; Get the value
    out     dx,al            ; Write to VGA
    inc     bp               ; Point to next cel
    loop    WriteAll         ; Loop while cx > 0
    sub     bp,768           ; Point to start array again
    pop     cx               ; Restore loopcounter from stack
    loop    Loop64           ; Have we done enough?
    sti                      ; Enable interrupts
    ret                      ; Return to main program
FadeOut ENDP


/*
SavePalette PROC NEAR        ; Save entire palette into an array
    cli                      ; Clear interrupts
    lea     bp,Parray        ; bp points to offset Palette array
    mov     dx,03c7h         ; Read register
    xor     al,al            ; Set al to 0 (start reading at color 0)
    out     dx,al            ; Give info to VGA
    mov     dx,03c9h         ; Data register
    mov     cx,768           ; Save all colors (256*3)
Saving:
    in      al,dx            ; Get what's in the register (read)
    and     al,00111111b     ; Mask of the upper 2 bits (value=0..63)
    mov     byte ptr [bp],al ; Save value into array
    mov     al,0             ; Set value to 0
    mov     byte ptr [bp+768],al  ; And save into second layer of array
    inc     bp               ; Point to next cel in aray
    loop    Saving           ; And loop while cx > 0
    sti                      ; Enable interrupts again
    ret
SavePalette ENDP
 */