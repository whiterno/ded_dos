.model tiny
.code
.186
locals @@
org 100h

HOT_KEY		equ 	33
VIDEOSEG	equ	0b800h
REG_AMOUNT  equ 12

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
X_coord		dw 0
Y_coord		dw 0
RegValues   db "AX 0000 BX 0000 CX 0000 DX 0000 SI 0000 DI 0000 BP 0000 SP 0000 DS 0000 CS 0000 ES 0000 SS 0000"
Buffer      dw 176 dup(0)

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
	jne cs:@@SkipActivate

	in al, 60h

	cmp al, HOT_KEY			; compare al and 33(F scancode)
	jne cs:@@SkipActivate

    mov al, cs:ActivateFlag             ; save old ActivateFlag
	xor cs:ActivateFlag, 00000001b		; ActivateFlag xor 1

    cmp al, 1
    je cs:@@SetFrame

	mov ax, 03h
	int 33h

	shr cx, 3
	shr dx, 3

	mov cs:X_coord, cx
	mov cs:Y_coord, dx

    call cs:saveFrame
    jmp cs:@@SkipActivate

@@SetFrame:
    call cs:setFrame

@@SkipActivate:

	pop es ds si di dx cx bx ax		; return all registers

	iret
	endp

;-----------------------------------------------------------------------------
; If ActivateFlag is 1 - draws a frame with real time registers' values
; every tick, then returns to old 08h handler
; If ActivateFlag is 0 - returns to old 08h handler
;-----------------------------------------------------------------------------
draw    	proc
	push bx ss es cs ds sp bp di si dx cx bx ax

	push cs
	pop ds

	cmp cs:ActivateFlag, 0
  	je cs:@@SkipDraw

    mov di, cs:Y_coord              ; y coord of the left-top corner of frame
    mov dx, cs:X_coord		; x coord
	call cs:firstCharAddr

	mov ax, VIDEOSEG        ; set videosegment address
	mov es, ax              ; in es

	mov ah, 00000111b       ; black back, red front
	mov bx, REG_AMOUNT + 4  ; set frame height
	mov dx, 11              ; set frame width
	mov si, offset cs:Style        ; set frame style

	call cs:drawFrame

    mov si, offset cs:RegValues
    call cs:writeAllReg

    call cs:screenReg

@@SkipDraw:

    pop ax bx cx dx si di bp bx ds bx es bx bx

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

;-----------------------------------------------------------------------------
;Write registers' values in string RegValues
;Entry:	SI - address of the beginning of the string
;Exit: None
;Destr: SI, AX, DX, CX, BX
;-----------------------------------------------------------------------------
writeAllReg proc
    mov cx, REG_AMOUNT
    mov bx, sp
    add bx, 2

@@Next:
    mov ax, word ptr ss:[bx]
    add si, 3

@@FirstDigit:
    mov dx, ax
    shr dx, 12

    cmp dl, 9
    jg cs:@@Hex1

    add dl, 30h
    mov byte ptr cs:[si], dl
    inc si
    jmp cs:@@SecondDigit
@@Hex1:
    add dl, 37h
    mov byte ptr cs:[si], dl
    inc si

@@SecondDigit:
    mov dx, ax
    shl dx, 4
    shr dx, 12

    cmp dl, 9
    jg cs:@@Hex2

    add dl, 30h
    mov byte ptr cs:[si], dl
    inc si
    jmp cs:@@ThirdDigit
@@Hex2:
    add dl, 37h
    mov byte ptr cs:[si], dl
    inc si

@@ThirdDigit:
    mov dx, ax
    shl dx, 8
    shr dx, 12

    cmp dl, 9
    jg cs:@@Hex3

    add dl, 30h
    mov byte ptr cs:[si], dl
    inc si
    jmp cs:@@FourthDigit
@@Hex3:
    add dl, 37h
    mov byte ptr cs:[si], dl
    inc si

@@FourthDigit:
    mov dx, ax
    shl dx, 12
    shr dx, 12

    cmp dl, 9
    jg cs:@@Hex4

    add dl, 30h
    mov byte ptr cs:[si], dl
    jmp cs:@@DoneWriting
@@Hex4:
    add dl, 37h
    mov byte ptr cs:[si], dl

@@DoneWriting:
    add bx, 2
    add si, 2
    dec cx

    cmp cx, 0
    je cs:@@Done
    jmp cs:@@Next
@@Done:

    ret
    endp

;-----------------------------------------------------------------------------
;Prints RegValues in the frame
;Entry:	None
;Exit: None
;Destr:
;-----------------------------------------------------------------------------
screenReg   proc
    mov di, cs:Y_coord              ; y coord of the left-top corner of frame
	mov dx, cs:X_coord		; x coord
    call cs:firstCharAddr

    add di, 324
    mov ah, 00000111b       ; black back, white front
    mov si, offset cs:RegValues
    mov dx, 7
    mov bp, REG_AMOUNT

@@Next:
    mov cx, dx
@@Loop:
    lodsb
    stosw

    loop cs:@@Loop
    inc si

    SHIFT_TO_NEXT_LINE

    dec bp
    cmp bp, 0
    jne cs:@@Next

    ret
    endp

;-----------------------------------------------------------------------------
;Save frame to Buffer
;Entry:	None
;Exit: None
;Destr:
;-----------------------------------------------------------------------------
saveFrame   proc
    mov di, cs:Y_coord              ; y coord of the left-top corner of frame
	mov dx, cs:X_coord		; x coord
    call cs:firstCharAddr

    mov si, offset cs:Buffer

    mov ax, VIDEOSEG
    mov es, ax

    mov cx, 16      ;height
    mov dx, 11      ;width for  macro
@@Next:
    mov bx, dx      ;width

@@Line:
    mov ax, word ptr es:[di]
    add di, 2

    mov word ptr cs:[si], ax
    add si, 2

    dec bx
    cmp bx, 0
    jne cs:@@Line

    SHIFT_TO_NEXT_LINE

    loop cs:@@Next

    ret
    endp

;-----------------------------------------------------------------------------
;Set frame from Buffer
;Entry:	None
;Exit: None
;Destr:
;-----------------------------------------------------------------------------
setFrame   proc
    mov di, cs:Y_coord              ; y coord of the left-top corner of frame
	mov dx, cs:X_coord		; x coord
    call cs:firstCharAddr

    mov si, offset cs:Buffer

    mov ax, VIDEOSEG
    mov es, ax

    mov ax, cs
    mov ds, ax

    mov cx, 16      ;height
    mov dx, 11      ;width for  macro
@@Next:
    mov bx, dx      ;width

@@Line:
    lodsw
    stosw

    dec bx
    cmp bx, 0
    jne cs:@@Line

    SHIFT_TO_NEXT_LINE

    loop cs:@@Next

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
