.model tiny
.code
locals @@
org 100h
Start:
		mov ax, 10      	; y coor
		mov si, 30		; x coor
		mov dh, 30		; width
		mov dl, 0		; initial shift
		
		mov di, 5		; height
		call drawFrame		; draw 1 line

		mov ax, 4c00h
		int 21h			; exit(0)

;-----------------------------------------------------------------------------
;Draws a line 122...223 with first 3 symbols (1,2,3) in given string
;Entry:	SI - character addr
;	DH - width
;	DL - shift (to given string)
;Exit:  None
;Destr: AX, BX, CX, DX, SI, ES
;-----------------------------------------------------------------------------
drawLine	proc

		;set shift from dl to bx
		mov bl, dl
		mov bh, 00h

		mov ah, byte ptr [CLR_ATTRS]		; ah = *(CLR_ATTRS)
		mov al, byte ptr [bx + SYMBOLS]		; al = *(shift + SYMB)
		mov word ptr es:[si], ax		; es:[si] = ax

		;set width from dh to dx
		mov dl, dh				; dl = dh
		mov dh, 00h				; dh = 00h
		
		;width--
		;change shift (to 2 in given string)
		;inc character address (2 bytes per character)  
		dec dx					; dx--
		inc bx					; bx++
		add si, 2				; si += 2

		;change ax character (to 2 in given string)
		mov al, byte ptr [bx + SYMBOLS]		; al = *(shift + SYMB)
		
		jmp @@Condition
Draw_Middle_Line:	
		;set character and increase adr pointer 
		mov word ptr es:[si], ax		; es:[si] = ax
		add si, 2				; si += 2
		dec dx					; width--
@@Condition:	cmp dx, 1				; dx ? 1
		ja Draw_Middle_Line			; if dx > 1 jump
		
		;set last symbol
		inc bx					; shift++
		mov al, byte ptr [bx + SYMBOLS]		; al = *(shift + SYMB)
		mov word ptr es:[si], ax	

@@Exit:		ret
		endp

;------------------------------------------------------------------------------
;Draws frame of symbols:
;takes first 9 symbols from given string and draws frame 122..223
;					      		455..556
;							455..556
;							........
;							455..556
;							788..889
;Entry:	AX - y coord
;	SI - x coord
;	DH - width
;	DI - height
;Exit:	None
;Destr: AX, BX, CX, DX, SI, DI, BP
;------------------------------------------------------------------------------
drawFrame	proc
		
		;save dx
		mov dl, 00h				; dl = 0
		mov bp, dx				; bp = dx (dh * 64)

		;set es to modify videosegment
		mov cx, VIDEOSEG			; cx = VIDEOSEG
		mov es, cx				; es = cx

		;save dx
		mov cx, dx				; cx = dx

		;get ax-line si-coord character addr
		mov bx, 80				; bx = 80
		mul bx					; ax *= bx
		add ax, si				; ax += si
		shl ax, 1				; ax << 1

		;revive dx
		mov dx, cx				; dx = cx

		;set shift from dl to bx
		mov bl, dl				; bl = dl
		mov bh, 00h				; bh = 0

		;set memory-friendly register
		mov si, ax				; si = ax

		;draw top line
		call drawLine
		
		;dec height, inc shift for symbols
		dec di					; di--
		add bp, 3				; bp += 3

		jmp @@Condition
Draw_Middle_Lines:
		;set dx for func
		mov dx, bp				; dx = bp

		;set addr shift for next line 
		mov ax, bp				; ax = bp
		shr ax, 8				; ax >> 8 
		dec ax					; ax--
		shl ax, 1				; ax << 1

		;shift addr
		add si, 160				; si += 160
		sub si, ax				; si -= ax

		call drawLine
		
		;dec height
		dec di					; di--

@@Condition:	cmp di, 1
		ja Draw_Middle_Lines			; if di > 1 jump
		
		;set addr shift for final line
		mov ax, bp				; ax = bp
		shr ax, 8				; ax >> 8
		dec ax					; ax--
		shl ax, 1				; ax << 1

		add si, 160				; si += 160
		sub si, ax				; si -= ax

		;set final shift for symbols		
		add bp, 3				; bp += 3
		mov dx, bp				; dx = bp

		call drawLine
		
@@Exit:		ret
		endp

;-----------------------------------------------------------------------------
;Calculates string's max lengh line and amount of \n
;Entry: MESSAGE
;Exit:	AH - amount of \n
;	AL - length 
;Destr:	AX
;-----------------------------------------------------------------------------
msgAnalyse	proc

Count_Max_Len:
		

@@Exit:		ret
		endp


VIDEOSEG	equ 0b800h

SYMBOLS:	db "+-+|.|+-+"
CLR_ATTRS:	db 00110100b
MESSAGE:	db "Sweet Valentine 14th!", '$'

end		Start