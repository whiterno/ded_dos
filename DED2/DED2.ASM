.model tiny
.code
.186
locals @@
org 100h

VIDEOSEG	equ	0b800h

Main:
	xor ax, ax		                ; ax = 0
	mov es, ax		                ; es = 0

	mov bx, 09h * 4		            ; 09h interruption
				                    ; 4 bytes for address

	mov ax, es:[bx]                 ;
	mov Old09Ofs, ax                ; save old int09h address
	mov ax, es:[bx + 2]             ;
	mov Old09Seg, ax                ;

	cli			                    ; if = 0 (clear if)

	mov es:[bx], offset newInt09	; set offset (fow newInt09)
                                    ; remember about little endian
                                    ; because of it in memory: offset:segment

	push cs				            ; ax = cs
	pop ax				            ;

	mov es:[bx + 2], ax		        ; set segment address

	sti			                    ; if = 1 (set if)

	;int 09h

	mov ah, 31h		                ; exit with reserving memory for resident
	mov dx, offset EOF
	shr dx, 4		                ; 31h counts in paragraphs
	inc dx                          ; to make sure everything in memory
	int 21h                         ; exit(with resident state)

newInt09	proc

	push ax bx cx dx si di ds es	; save registers not to fuck them up

    mov ax, VIDEOSEG                ; set videomemory
    mov es, ax                      ; in es

	mov ah, 4eh                     ; red back - yellow fore
	mov di, 5 * 80 * 2              ; 5th line

	cld                             ; df = 0 (++)

    in al, 60h                      ; al = scancode
    stosw                           ; es:[di++] = ax

	in al, 61h		;
	mov ah, al		; blink enable/disable keyboard
	or al, 80h		; to let keyboard controller know
	out 61h, al		; that changes done
	mov al, ah		;
	out 61h, al		; Keyboard Controller needs it

	mov al, 20h		; send End of Interrupt signal
	out 20h, al		; Interrupt Controllet needs it

	pop es ds di si dx cx bx ax		; revive registers
				                    ; done your deel - return former registers

ChainOldISR:                        ; address of the old int09h
	db 0eah                         ;   jmp in bytes
Old09Ofs	dw 0                    ;   offset - changes in line 17
Old09Seg	dw 0                    ;   segment - changes in line 19

	iret
	endp

EOF:
end		Main
