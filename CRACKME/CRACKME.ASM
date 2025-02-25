.model tiny
.code
org 100h
locals @@

PASSWORD_LEN	equ 3
CONTROL_BYTE	equ 0dh

Start:
    	mov ah, 09h
    	mov dx, offset Password_Request
    	int 21h

	mov cx, 0
    	mov di, offset Password_User
ReadSymb:
	add cx, 1000h
    	mov ah, 01h
	int 21h

    	stosb

	cmp al, 0dh
	jne ReadSymb

	shr cx, 12
	cmp cx, PASSWORD_LEN + 1
	jne @@Fail

	call checkPasswords

	cmp al, 1
	jne @@Fail

	mov ah, 09h
	mov dx, offset Password_Success
	int 21h

	jmp @@Exit

@@Fail:

	mov ah, 09h
	mov dx, offset Password_Fail
	int 21h	

@@Exit:
    	mov ax, 4c00h       ;
    	int 21h             ; exit(0)

;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
checkPasswords	proc

	mov si, offset Password_User
	mov di, offset Password_Real

	mov al, 1
	mov cx, PASSWORD_LEN

@@Next:
	
	mov dl, byte ptr [si]
	mov dh, byte ptr [di]
	
	inc si
	inc di

	cmp dl, dh
	je @@Good
	mov al, 0
@@Good:
	loop @@Next
	
	ret
	endp

Password_Request    db "Please, insert password to continue working with system:", 0ah, 0dh, "$"
Password_Success    db "Access permited", 0ah, 0dh, "$"
Password_Fail	    db "You won't fool me", 0ah, 0dh, "$"

Password_User	    db PASSWORD_LEN + 1 dup(0)
Password_Real	    db "123"
end     Start