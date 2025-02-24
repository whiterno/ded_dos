.model tiny
.code
org 100h

Start:		mov ah, 09h			; DOS ah = puts(dx)
		sub si, ah
		lea dx, String[1]
		int 21h				; puts(dx)
.data
 String: 	db "Hello", '$'
.code
		mov ax, 4c00h			; DOS ax = exit(0)
		int 21h				; exit(0) 

end		Start