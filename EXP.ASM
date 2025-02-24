.model tiny
.code
org 100h

Start:
		mov ah, 01h
		int 21h
		mov dl, al
		mov ah, 02h
		int 21h

		mov ax, 4c00h
		int 21h

String	 	db "Hello!", 0dh, 0ah, '$'
As		dw String
end		Start