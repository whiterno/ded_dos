.model tiny
.code
org 100h

Start:
	mov ax, 1111h
	mov bx, 2222h
	mov cx, 3333h
	mov dx, 4444h

	jmp Start

end	Start