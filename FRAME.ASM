.model tiny
.code
.186
locals @@
org 100h

;-----------------------------------------------------------------------------
;Changes DI address to the beginning of the next line of frame
;Entry: DI - address pointer
;	DX - line width
;Exit: 	DI - address of the beginning of the next line of frame
;Destr:	DI
;-----------------------------------------------------------------------------
SHIFT_TO_NEXT_LINE macro
 		sub di, dx		; shift in line
		sub di, dx		; di = di - 2*dx
		add di, 160		; jump on the next line
endm

VIDEOSEG	equ	0b800h

;-----------------------------------------------------------------------------
;Counts string length until '$' (not including)
;Entry: DI - memory address of the string
;Exit:	CX - length of the string
;Destr: AL, CX, DI
;-----------------------------------------------------------------------------
STR_LEN		macro
		mov al, '$'		; set terminating symbol
		xor cx, cx		; cx = 0
		dec cx			; cx--
		repne scasb		; while ((cx--) and al != es:[di])
		neg cx			; cx = -cx
		dec cx			; cx--
endm

;-----------------------------------------------------------------------------
;Clears screen
;Entry: None
;Exit:	None
;Destr:	AH, DX
;-----------------------------------------------------------------------------
CLEAR_SCREEN	macro
		mov ah, 09h
		mov dx, offset CLEAR_VIEW
		int 21h
endm

Start:
		jmp Main

;-----------------------------------------------------------------------------
include cmd.asm
include strings.asm
;-----------------------------------------------------------------------------

Main:
		CLEAR_SCREEN

		mov si, 81h		; si = 81h (start of the cmd line mem)

		call skipSpaces		; get width from cmd line
		call getDec		;

		mov dx, ax		; dx = ax

		call skipSpaces		; get height from cmd line
		call getDec		;

		mov di, ax		; di = ax

		mov bx, di		;
		shl bx, 8		; bx = height:width (from cmd)
		mov bl, dl		;

		call colorStyle 	; set STYLE and COLOR_ATTR

		mov MESSAGE, si	 	; MESSAGE = si

		push bx			; save bx

		mov di, MESSAGE		; di = MESSAGE
		call maxStrLenNum

		add bh, 6
		add bl, 6

		pop ax			; ax = bx (before maxStrLenNum)
		push bx			; save frame MESSAGE height:width

		cmp al, bl			; if (al > bh)
		jle @@Width_Done 		; then
		mov bl, al			; bl = al

@@Width_Done:

		cmp ah, bh			; if (ah > bh)
		jle @@Height_Done		; then
		mov bh, ah			; bh = ah

@@Height_Done:

		xor dx, dx		;
		mov dl, bl		; dx = frame width

		xor di, di		;
		mov di, bx		; di = frame height
		shr di, 8		;

		pop ax			; ax = height:width for MESSAGE frame
		
		call frameUltimate

;-----------------------------------------------------------------------------
;Counts memory address of the character on x and y coords in videosegment
;Entry: DX - x coord
;	DI - y coord
;Exit:	DI - memory adress of the character
;Destr: DI, AX
;-----------------------------------------------------------------------------
firstCharAddr	proc

		mov ax, di			;
		shl di, 6			;
		shl ax, 4		 	; count memory adress
		add di, ax			;
		add di, dx			;
		shl di, 1 			;

		ret
		endp

;-----------------------------------------------------------------------------
;Counts x and y coords of the beginning of the frame for it to be centered
;with given width and height
;Entry:	DX - width
;	DI - height
;Exit:	DX - x coord
;	DI - y coord
;Destr: DX, DI
;-----------------------------------------------------------------------------
centerXY	proc

		neg dx			;
		add dx, 80		; x coord of left-top corner
		shr dx, 1		;

		neg di			;
		add di, 25		; y coord of left-top corner
		shr di, 1		;

		ret
		endp

;-----------------------------------------------------------------------------
;Draws a line of characters in 3 byte string (122..223)
;Entry:	DI - memory address of the 1st character in videosegment
;	SI - memory address of 3 byte string
;	AH - color attribute
;	CX - line width
;Exit:	None
;Destr: DI, SI, CX, AL
;-----------------------------------------------------------------------------
drawLine	proc

		lodsb				; draw 1st character
		stosw				;

		sub cx, 2			;
		lodsb				; draw middle part (2nd chars)
		rep stosw			;

		lodsb				; draw 3rd character
		stosw				;

		ret
		endp

;-----------------------------------------------------------------------------
;Draws a frame of characters in 9 byte string
;Entry:	DI - memory address of the 1st character of the frame
;	AH - color attribute
;	BX - frame height
;	DX - line width
;	SI - memory address of 9 byte string
;Exit:	DI - memory address of the 1st character of the frame
;	DX - frame width
;Destr: AL, BX, CX, SI, DI, BP
;-----------------------------------------------------------------------------
drawFrame	proc

		mov bp, di			; save di for strings

		mov cx, dx			; draw 1st line
		call drawLine			;

		sub bx, 2
@@Next:
		SHIFT_TO_NEXT_LINE

		mov cx, dx			; revive width

		call drawLine

		sub si, 3			; return to 456 chars in str
		dec bx				; height--
		cmp bx, 0
		ja @@Next

		SHIFT_TO_NEXT_LINE

		add si, 3			; *(si) = 7 (in 9 bytes str)
		mov cx, dx			; revive width

		call drawLine

		mov di, bp			; revive di

		ret
		endp

;-----------------------------------------------------------------------------
;Sets COLOR_ATTR and STYLE
;Entry: SI - addressed on color index (or spaces before it)
;Exit:	SI - addressed on message
;Destr:	AX, SI, DX,
;-----------------------------------------------------------------------------
colorStyle	proc

		call skipSpaces		;
		call getHex		; get color attribute
		mov COLOR_ATTR, al	;

		call skipSpaces		;
		cmp byte ptr[si], '0'	; get style index
		jne @@Set_Style		;

		inc si
		call skipSpaces

		mov STYLE, si		; save style address
		add si, 9		; skip style for message
 		jmp @@Style_Done

@@Set_Style:
		xor dx, dx		;
		mov dl, byte ptr[si]	; get style index - 1
		sub dl, '1'		;

		mov ax, dx			;
		shl dx, 3			;
		add dx, ax			; get style address
		mov STYLE, dx			;
		add STYLE, offset SET_STYLES	;
		inc si				;
		call skipSpaces
@@Style_Done:

		ret
		endp

;-----------------------------------------------------------------------------
;Counts coords, draws frame and prints MESSAGE
;-----------------------------------------------------------------------------
frameUltimate proc
		push ax			; save height:width for MESSAGE frame
		push dx			; save frame width

		call centerXY		; find left-top corner coords
					; to center the frame

		call firstCharAddr	; find left-top corner address

		push VIDEOSEG		; set video segment address
		pop es			; in es: es = b800h

		mov ah, COLOR_ATTR 	; revive color attribute
		mov si, STYLE		; si = STYLE
		pop dx			; revive frame width
		shr bx, 8		; bx (height:width->00:height)

		call drawFrame

		pop ax			; revive height:width for MESSAGE frame
		sub ah, 6		; 
		mov al, dl		; center by height in frame
		push dx			; save frame width
		xor di, di		; di = 0
		xor dx, dx		; dx = 0

		mov dl, al		; dl = al (width)
		mov di, ax		; di = ax (height:width)
		shr di, 8		; di >> 8 (00:height)

		push di			; save amonut of strings

		call centerXY
		call firstCharAddr

		mov si, MESSAGE		; si = MESSAGE

		pop bx				; bx = amount of strings
		pop dx				; dx = frame width

		call printStrs		; print MESSAGE

		mov ax, 4c00h
		int 21h			; exit(0)

		ret
		endp

COLOR_ATTR	db ?
STYLE		dw ?
MESSAGE		dw ?
SET_STYLES	db 218d, 196d, 191d, 179d, 32d, 179d, 192d, 196d, 217d, \
		201d, 205d, 187d, 186d, 32d, 186d, 200d, 205d, 188d,	\
		3d, 3d, 3d, 3d, 32d, 3d, 3d, 3d, 3d,			\
		"+-+I I+-+",						\
		"+=+I I+=+",						\
		"1234 6789",						\
		16d, 22d, 17d, 16d, 32d, 17d, 16d, 22d, 17d,		\
		3d, 3d, 3d, 4d, 32d, 5d, 6d, 6d, 6d,			\
		1d, 1d, 1d, 2d, 32d, 2d, 1d, 1d, 1d

CLEAR_VIEW	db 0ah, 0ah, 0ah, 0ah, 0ah, 0ah, 0ah, 0ah, 0ah, 0ah, 0ah,  \
		0ah, 0ah, 0ah, 0ah, 0ah, 0ah, 0ah, 0ah, 0ah, 0ah, 0ah,  \
		0ah, 0ah, 0ah, 0ah, '$'

end 		Start
