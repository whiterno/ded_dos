.model tiny
.code
org 100h

Start:
	mov cx, 0fdcdh
	mov ah, 02h
	mov dl, 30h
Next:
	int 21h
	loop Next

	mov dl, 1fh
	int 21h

	mov dl, 01h
	int 21h

	mov ax, 4c00h
	int 21h

end	Start