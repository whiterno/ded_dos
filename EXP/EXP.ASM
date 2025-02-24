.model tiny
.code
.186
org 100h

Start:
	push ax bx

	call func
	
	mov ax, 4c00h
	int 21h

func	proc

	push ax

	ret
	endp

end	Start
