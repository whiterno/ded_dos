.model tiny
.code
org 100h

SYSTEM_SEG	equ 	0h

Start: 
	xor ax, ax
	mov es, ax

	mov bx, 09h * 4

	cli

	mov es:[bx], offset IntNine
	mov ax, cx
	mov es:[bx + 2], ax

	sti

	mov ah, 31h
	mov dx, THE_END
	

IntNine:
	
	