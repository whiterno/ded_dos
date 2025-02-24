.model tiny
.code
.186
org 100h

HOT_KEY		equ 	33
VIDEOSEG	equ	0b800h

;-----------------------------------------------------------------------------
;Changes DI address to the beginning of the next line of frame
;Entry: DI - address pointer
;	DX - line width
;Exit: 	DI - address of the beginning of the next line of frame
;Destr:	DI
;-----------------------------------------------------------------------------
SHIFT_TO_NEXT_LINE macro
 		sub di, dx		; shift in line
		sub di, dx		; di = di - 2*dx
		add di, 160		; jump on the next line
endm

Start:

	jmp Load

ActivateFlag	db 0
Old08Handler	dd 0
Old09Handler	dd 0

Style		db 201d, 205d, 187d, 186d, 32d, 186d, 200d, 205d, 188d


;----------------------------------------------------------------------------
; New 09h interrupt, checks combination CTRL-F and if pushed makes xor 1 to
; ActivateFlag
;----------------------------------------------------------------------------
checkKeys	proc
	pushf
	call cs:Old09Handler

	push ax bx cx dx di si ds es
	
	mov ah, 02h
	int 16h

	cmp al, 04h		; check if ctrl is pushed
	jne cs:SkipActivate

	in al, 60h

	cmp al, HOT_KEY			; compare al and 33(F scancode)
	jne cs:SkipActivate

	xor cs:ActivateFlag, 00000001b		; ActivateFlag xor 1

SkipActivate:

	pop es ds si di dx cx bx ax		; return all registers

	iret
	endp

;-----------------------------------------------------------------------------
; If ActivateFlag is 1 - draws a frame with real time registers' values
; every tick, then returns to old 08h handler
; If ActivateFlag is 0 - returns to old 08h handler
;-----------------------------------------------------------------------------
draw    	proc
	push ax bx cx dx di si es ds bp

	push cs
	pop ds

	cmp cs:ActivateFlag, 0
  	je cs:SkipDraw

    	mov dx, 4               ; x
    	mov di, 4               ; y coords of the left-top corner of frame
    	call cs:firstCharAddr

    	mov ax, VIDEOSEG        ; set videosegment address
    	mov es, ax              ; in es

    	mov ah, 00000111b       ; black back, red front
    	mov bx, 8               ; set frame height
    	mov dx, 11              ; set frame width
    	mov si, offset cs:Style        ; set frame style

	call cs:drawFrame

SkipDraw:

    	pop bp ds es si di dx cx bx ax

	jmp cs:Old08Handler

	endp

;-----------------------------------------------------------------------------
;Counts memory address of the character on x and y coords in videosegment
;Entry: DX - x coord
;	DI - y coord
;Exit:	DI - memory adress of the character
;Destr: DI, AX
;-----------------------------------------------------------------------------
firstCharAddr	proc

	mov ax, di			;
	shl di, 6			;
	shl ax, 4		 	; count memory adress
	add di, ax			;
	add di, dx			;
	shl di, 1 			;

	ret
	endp

;-----------------------------------------------------------------------------
;Draws a line of characters in 3 byte string (122..223)
;Entry:	DI - memory address of the 1st character in videosegment
;	SI - memory address of 3 byte string
;	AH - color attribute
;	CX - line width
;Exit:	None
;Destr: DI, SI, CX, AL
;-----------------------------------------------------------------------------
drawLine	proc

	lodsb				; draw 1st character
	stosw				;

	sub cx, 2			;
	lodsb				; draw middle part (2nd chars)
	rep stosw			;

	lodsb				; draw 3rd character
	stosw				;

	ret
	endp

;-----------------------------------------------------------------------------
;Draws a frame of characters in 9 byte string
;Entry:	DI - memory address of the 1st character of the frame
;	AH - color attribute
;	BX - frame height
;	DX - line width
;	SI - memory address of 9 byte string
;Exit:	DI - memory address of the 1st character of the frame
;	DX - frame width
;Destr: AL, BX, CX, SI, DI, BP
;-----------------------------------------------------------------------------
drawFrame	proc

	mov bp, di			; save di for strings

	mov cx, dx			; draw 1st line
	call cs:drawLine		;

	sub bx, 2
@@Next:
	SHIFT_TO_NEXT_LINE

	mov cx, dx			; revive width

	call cs:drawLine

	sub si, 3			; return to 456 chars in str
	dec bx				; height--
	cmp bx, 0
	ja cs:@@Next

	SHIFT_TO_NEXT_LINE

	add si, 3			; *(si) = 7 (in 9 bytes str)
	mov cx, dx			; revive width

	call cs:drawLine

	mov di, bp			; revive di

	ret
	endp

EndOfResident:

Load:
	mov ax, 3509h			; get old 09h interrupt
	int 21h				; handler in es:bx

	mov word ptr Old09Handler, bx		; save old 09h offset
	mov word ptr Old09HAndler + 2, es	; save old 09h segment

	mov ax, 2509h				; set new 09h interrupt
	mov dx, offset checkKeys		; handler address
	int 21h					;

	mov ax, 3508h				; get old 08h interrupt 
	int 21h					; handler addr in es:bx

	mov word ptr Old08Handler, bx		; save old 08h offset
	mov word ptr Old08Handler + 2, es	; save old 08h segment

	mov ax, 2508h				; set new 08h interrup
	mov dx, offset draw			; handler address
	int 21h					;

	mov ax, 3100h				;
	mov dx, offset EndOfResident		; dx = &EndOfResident
	shr dx, 4				; count in paragraphs
	inc dx					;
	int 21h					; save and stay resident

end	Start
