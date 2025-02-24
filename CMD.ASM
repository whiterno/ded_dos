;-----------------------------------------------------------------------------
;Skips spaces in cmd
;Entry: SI - address of space
;Exit:  SI - address of the next not space character
;Destr: SI
;-----------------------------------------------------------------------------
skipSpaces  proc

@@Next:
        cmp byte ptr[si], 20h
        jne @@Done
        inc si
        jmp @@Next

@@Done: ret
        endp

;-----------------------------------------------------------------------------
;Gets decimal number from command line
;Entry: SI - number address
;Exit:  AX - decimal number
;Destr: AX, SI, CX
;-----------------------------------------------------------------------------
getDec  proc

        xor ax, ax

@@Next:
        add al, byte ptr[si]
        inc si
        xor cx, cx

        sub ax, '0'
        add cx, ax
        shl cx, 2
        add ax, cx
        shl ax, 1

        cmp byte ptr [si], 20h
        jne @@Next

        shr ax, 1
        sub ax, cx

        ret
        endp

;-----------------------------------------------------------------------------
;Gets hexadecimal number from command line
;Entry: SI - number address
;Exit:  AX - hexadecimal number
;Destr: AX, SI
;-----------------------------------------------------------------------------
getHex  proc

        xor ax, ax

@@Next:
        add al, byte ptr[si]

        cmp byte ptr[si], 'a'
        jge @@Hex
        jmp @@Dec

@@Hex:
        inc si

        sub ax, 'a'
	add ax, 10
        shl ax, 4

        jmp @@Condition

@@Dec:
        inc si

        sub ax, '0'
        shl ax, 4

@@Condition:cmp byte ptr[si], 20h
            jne @@Next

        shr ax, 4

        ret
        endp
