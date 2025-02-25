.model tiny
.code
org 100h

Start:
	
Next:
	in al, 60h

	mov bx, 2222h
	mov cx, 3333h
	mov dx, 4444h
	mov si, 5555h
	mov di, 6666h
	mov bp, 7777h
	
	mov es, bx
	mov ds, bx

	cmp al, 1
	jne Next

	mov ax, 4c00h
	int 21h

end	Start