;-----------------------------------------------------------------------------
;Counts max length of given strings (divided by '$') and their amount
; "$$" means full stop
;Entry: DI - memory address of the string
;Exit:	BH:BL - amount of strings : max length of the string
;Destr:	DI, BX, CX, BP, AL
;-----------------------------------------------------------------------------
maxStrLenNum	proc

		xor bx, bx			; bx = 0
@@Next:
		STR_LEN				; length of string untill '$'
		dec cx				; remove '$'
		inc bh				; amount++

		cmp bl, cl
		jae @@Else
		mov bl, cl			; bl = cl if (bl < cl)

@@Else:		cmp cl, 0
		jne @@Next

		dec bh				; amount -- (fictive line)

		ret
		endp

;-----------------------------------------------------------------------------
;Prints string until '$' centered in line
;Entry: DI - destination address
;		SI - string address
;       DX - frame width
;Exit:	DI - not changed
;Destr:	SI, BP, CX, AL
;-----------------------------------------------------------------------------
printStrCent	proc

		mov bp, di			; bp = di

		mov di, si			; di = si

		mov ax, ds			; set es from video segment
		mov es, ax			; to data segment
		STR_LEN
		dec cx

		mov di, bp			; di = bp

		neg cx				; cx - x shift relative to frame
		add cx, dx			;
		shr cx, 1			;

		mov bp, di			; save di

		add di, cx			; x shift
		add di, cx 			;

		mov ax, VIDEOSEG		; return es to
		mov es, ax			; video segment

@@Next:
		movsb				; es:[di++] = ds:[si++]
		inc di				; di++

		cmp byte ptr [si], '$'
		jne @@Next

		inc si				; si++
		cmp byte ptr[si], '$'
		jne @@Done

		mov ax, 4c00h
		int 21h
@@Done:
		mov di, bp			; return in the start of the
						; line

		ret
		endp

;-----------------------------------------------------------------------------
;Prints set amount of strings divided by '$'
;Entry:	DI - destination address
;		SI - string address
;		BX - amount of strings
;       DX - frame width
;Exit:	None
;Destr: BX, DI, SI
;-----------------------------------------------------------------------------
printStrs	proc

@@Next:
		call printStrCent

		add di, 160		; jump on the next line
		dec bx			; bx--

		cmp bx, 0
		jg @@Next

		ret
		endp
