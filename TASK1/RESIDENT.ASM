.model tiny
.code
.186
locals @@
org 100h

VIDEOSEG	equ	0b800h

SHIFT_TO_NEXT_LINE	macro
	
	add di, 160
	sub di, 22

	endm

Main:
	xor ax, ax		; ax = 0
	mov es, ax		; es = 0

	mov bx, 09h * 4		; 09h interruption
				; 4 bytes for address
	mov ax, es:[bx]
	mov Old09Ofs, ax
	mov ax, es:[bx + 2]
	mov Old09Seg, ax
 
	cli			; if = 0 (clear if)

	mov es:[bx], offset newInt09	; set shift

	push cs				; ax = cs
	pop ax				;

	mov es:[bx + 2], ax		; set segment address

	sti			; if = 1 (set if)

	int 09h

	mov ah, 31h		; exit with reserving memory for resident
	mov dx, offset EOF	
	shr dx, 4		; 31h counts in paragraphs
	inc dx
	int 21h

newInt09	proc

	push ax bx cx dx si di ds es	; save registers not to fuck them up

	mov ah, 4eh
	mov di, 5 * 80 * 2

	cld

	in al, 61h		;
	mov ah, al		; blink enable/disable keyboard		
	or al, 80h		; to let keyboard controller know
	out 61h, al		; that changes done
	mov al, ah		;
	out 61h, al		; KC needs it

	mov al, 20h		; send EOI signal
	out 20h, al		; IC needs it

	;call drawFrame

	pop es ds di si dx cx bx ax		; revive registers
				; done our deel without attention
ChainOldISR:
	db 0eah
Old09Ofs	dw 0
Old09Seg	dw 0

	mov ax, VIDEOSEG
	mov es, ax

	mov ah, 00h
	int 16h
	mov ah, 4eh
	stosw

	iret
	endp

;-----------------------------------------------------------------------------
;Draws frame 6x11 with STYLE; left-top corner coordinates (65, 5)
;Entry: None
;Exit:  None
;Destr: 
;-----------------------------------------------------------------------------
drawFrame	proc
		
	mov ax, VIDEOSEG
	mov es, ax
	push cs
	pop ds

	mov ah, 7		; black back, white fore
	mov di, 930		; (65, 5)
	mov si, offset cs:STYLE	; set style
	
	call drawLine
	SHIFT_TO_NEXT_LINE

	mov bx, 9
@@Next:
	call drawLine

	sub si, 3
	SHIFT_TO_NEXT_LINE
	dec bx

	cmp bx, 0
	jne @@Next

	add si, 3
	call drawLine

	ret
	endp


;-----------------------------------------------------------------------------
;Draws a line of 11 characters in 3 byte string (12222222223)
;Entry:	DI - memory address of the 1st character in videosegment
;	SI - memory address of 3 byte string
;Exit:	None
;Destr: DI, SI, CX, AL
;-----------------------------------------------------------------------------
drawLine	proc
	
		mov cx, 9

		lodsb				; draw 1st character
		stosw				;

		lodsb				; draw middle part (2nd chars)
		rep stosw			;

		lodsb				; draw 3rd character
		stosw				;

		ret
		endp

STYLE:		db 218d, 196d, 191d, 179d, 32d, 179d, 192d, 196d, 217d

EOF:
end		Main