.model tiny
.code
.186
org 100h

HOT_KEY		equ 	33

Start:

	jmp Load

ActivateFlag	db 0
Old08Handler	dd 0
Old09Handler	dd 0


;----------------------------------------------------------------------------
; New 09h interrupt, checks combination CTRL-F and if pushed makes xor 1 to
; ActivateFlag
;----------------------------------------------------------------------------
checkKeys	proc
	push ax bx cx dx di si ds es

	in al, 60h			; read letter's scancode from 60h port
	mov ah, byte ptr 0000:[0417]

	and ah, 00000010b		; ah = 10b if ctrl is pushed,
					; 	0b otherwise

	cmp ah, 0			; check if ctrl is pushed
	jne cs:SkipActivate
	cmp al, HOT_KEY			; compare al and 33(F scancode)
	jne cs:SkipActivate

	xor cs:ActivateFlag, 00000001b		; ActivateFlag xor 1

SkipActivate:

	pop es ds si di dx cx bx ax		; save all registers

	jmp cs:Old09Handler			; return to old handler

	endp

;-----------------------------------------------------------------------------
; If ActivateFlag is 1 - draws a frame with real time registers' values
; every tick, then returns to old 08h handler
; If ActivateFlag is 0 - returns to old 08h handler 
;-----------------------------------------------------------------------------
drawFrame	proc
	push ax bx cx dx di si es ds

	


SkipDraw:

	jmp cs:Old08Handler

	endp

	

EndOfResident:

Load:
	mov ax, 3509h			; get old 09h interrupt
	int 21h				; handler in es:bx

	mov word ptr Old09Handler, bx		; save old offset
	mov word ptr Old09HAndler + 2, es	; save old segment

	mov ax, 2509h				; set new 09h interrupt
	mov dx, offset checkKeys		; handler address
	int 21h					;
	
	mov ax, 3100h				; 
	mov dx, offset EndOfResident		; dx = &EndOfResident
	shr dx, 4				; count in paragraphs
	inc dx					;
	int 21h					; save and stay resident 	

end	Start
	