;==============================================================================;
;                                                                              ;
;   Assembler program by Vulture.                                              ;
;                                                                              ;
;   This program will slowly fade-up a picture on the screen. Then it will     ;
;   wait for you to press escape before it fades the screen to black.          ;
;                                                                              ;
;   Pretty basic stuff here. But it's important to fully understand what       ;
;   is done here. When you get the idea, you can create your own cewl fx       ;
;   using the palette. Like palette rotations... Read the doc file for         ;
;   more info.                                                                 ;
;                                                                              ;
;   Greetinx,                                                                  ;
;                                                                              ;
;         Signed:   Vulture                                                    ;
;                                                                              ;
;==============================================================================;

DOSSEG        ; Sort segment like highlevel languages do
.MODEL SMALL  ; Code & data both < 64 kB, seperate segments
.STACK 200h   ; Define a 512 byte stack
.286          ; Allow 80286 instructions in your code
.DATA         ; Data segment starts here (empty)
.CODE         ; Code segment starts here
ASSUME cs:@code,ds:@code       ; cs and ds pointing to codesegment

; === DATA ===

INCLUDE Picture.dat          ; Picture file with colorvalues (lineair)
INCLUDE Palette.dat          ; Palette file with RGB values

Credits DB   13,10,"Code and gfx by Vulture.",13,10,"$" ; Important message :)

Parray  DB   768*2 DUP (?)   ; Array to hold 2 entire palettes

; === PROCEDURES ===

SetVGA PROC NEAR             ; Get into VGA mode
    mov     ax,0013h         ; Set the videomode 320*200*256
    int     10h              ; call VID interrupt
    ret                      ; Return to main program
SetVGA ENDP

SetText PROC NEAR            ; Get into character mode
    mov     ax,0003h         ; Set 80x25x16 char mode
    int     10h              ; call VID interrupt
    ret                      ; Return to main program
SetText ENDP

WaitVrt PROC NEAR            ; Waits for vertical retrace to reduce "snow"
    mov     dx,3dah
Vrt:
    in      al,dx
    test    al,1000b         ; Test 3rd bit
    jnz     Vrt              ; Wait until Verticle Retrace starts
NoVrt:
    in      al,dx
    test    al,1000b         ; Test 3rd bit again
    jz      NoVrt            ; Wait until Verticle Retrace ends
    ret                      ; Return to main program
WaitVrt ENDP

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

BlackOut PROC NEAR           ; Reset all R,G,B values to 0
    cli
    mov     dx,03c8h         ; Write register
    xor     al,al            ; Start at color 0 (and set R,G,B to 0 too)
    out     dx,al            ; Pass info to VGA
    mov     dx,03c9h         ; Data register
    mov     cx,768           ; Do all colors (256*3)
Reset:
    out     dx,al            ; Out all zero's to VGA
    loop    Reset            ; Loop while cx > 0
    sti
    ret
BlackOut ENDP

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

FadeIn PROC NEAR             ; Fades screen to desired colors
    cli                      ; Disable interrupts
    lea     bp,Parray        ; Load offset Palette array
    mov     cx,64            ; Set loopcounter
Loop64X:
    xor     bx,bx            ; Reset bx to 0
Loop768X:
    mov     dl,byte ptr [bp]        ; Store original value in dl
    mov     dh,byte ptr [bp+768]    ; And fade-up value in dh
    cmp     dl,dh                   ; Compare them
    je      NextOne                 ; If they are equal then do the next one
    inc     byte ptr [bp+768]       ; Else increase with 1 (second layer)
NextOne:
    inc     bp               ; Point to next arraycel
    inc     bx               ; Increase counter
    cmp     bx,768           ; Have we done all registers?
    jl      Loop768X
    push    cx               ; Save first loopcounter
    mov     cx,768           ; Do all R,G,B values
    call    WaitVrt          ; Wait for a vertical retrace
    call    WaitVrt          ; Twice
    mov     dx,03c8h         ; Write register
    xor     al,al            ; Start writing at color 0
    out     dx,al            ; Give info to VGA
    mov     dx,03c9h         ; Data register
WritemAll:
    mov     al,byte ptr [bp] ; Get the value (bp points to second palette)
    out     dx,al            ; Write to VGA
    inc     bp               ; Point to next cel
    loop    WritemAll        ; Loop while cx > 0
    sub     bp,768*2         ; Point to start palette again (reached the end)
    pop     cx               ; Restore loopcounter from stack
    loop    Loop64X          ; Have we done enough?
    sti                      ; Enable interrupts
    ret                      ; Return to main program
FadeIn ENDP

; === MAIN PROGRAM ===

START:                       ; Main program starts here

    call    SetVGA           ; Get into VGA mode

    mov     ax,cs            ; Move CS into AX
    mov     es,ax            ; es points to codesegment (data)
    mov     ax,1012h         ; Select write palette function
    mov     bx,0             ; Start at color 0
    mov     cx,255           ; Write 256 colors ( 0-255 )
    lea     dx,Palette       ; es:dx points to palette data
    int     10h              ; Call VID interrupt & set palette

    call    SavePalette      ; Save entire palette
    call    BlackOut         ; Set all R,G,B values to 0

; === Initialize pointers ===
    mov     ax,cs
    mov     ds,ax            ; ds points to codesegment (data)
    mov     ax,0a000h        ; Move VGA segment AX
    mov     es,ax            ; es points to VGA

; === Load the picture (not shown yet) ===
    mov     cx,32320d        ; Number of bytes in cx
    lea     si,Picture       ; ds:si => picture data
    mov     di,320*50        ; es:di => startpos on screen
    rep     movsb            ; Copy them all...

    call    FadeIn           ; Fade to original colors

Escape:
    in      al,60h           ; Scan keyboard
    cmp     al,1             ; Was escape hit?
    jne     Escape           ; If not, loop

    call    FadeOut          ; Fade screen to black
    call    SetText          ; Set textmode again

; === Print string and quit ===
    mov     ax,cs
    mov     ds,ax            ; ds points to cs (data)
    lea     dx,Credits       ; Get offset => ds:dx points to string
    mov     ah,9             ; Select function 9 in AH
    int     21h              ; Print string using ds:dx far-pointer
    mov     ax,4c00h         ; Give control to DOS (MUST BE HERE!!!)
    int     21h              ; Call interrupt and exit

END START                    ; End of C<><>L program...  :)




;  That's it. Hope it was understandable.
;  Use this in yar own programs but give credit where credit is due.
;  Took me a long time to figure thiz all out. Besides, a greet don't
;  cost ya nothing...... :)

;      Vulture.
