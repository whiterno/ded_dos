.model tiny
.code
org 100h

Start:	xor ax, ax
	
MK:	mov ax, MK

	mov ax, 4c00h
	int 21h

end	Start