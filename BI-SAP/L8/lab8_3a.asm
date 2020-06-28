; definice pro nas typ procesoru
.include "m169def.inc"
; podprogramy pro praci s displejem
.org 0x1000
.include "print.inc"

; Zacatek programu - po resetu
.org 0
jmp start

; Zacatek programu - hlavni program
.org 0x100
start:
    ; Inicializace zasobniku
	ldi r16, 0xFF
	out SPL, r16
	ldi r16, 0x04
	out SPH, r16
    ; Inicializace displeje
	call init_disp

    
	; zadání císel (0x87654321)
	ldi r18, 0x21
	ldi r19, 0x43
	ldi r20, 0x65
	ldi r21, 0x87

	ldi r22, 0x21
	ldi r23, 0x43
	ldi r24, 0x65
	ldi r25, 0x87

	; secteni cisel
	add r18, r22
	adc r19, r23
	adc r20, r24
	adc r21, r25

	; detekování overflow
	brvs over
	; detekování vypadnutí
	brlo carry
	; default
	jmp zero

end: jmp end        ; Zastavime program - nekonecna smycka

over:
	ldi r16, 'O'
	ldi r17, 2
	call show_char
	ldi r16, 'V'
	ldi r17, 3
	call show_char
	ldi r16, 'E'
	ldi r17, 4
	call show_char
	ldi r16, 'R'
	ldi r17, 5
	call show_char
	jmp end

carry:
	ldi r16, 'C'
	ldi r17, 2
	call show_char
	ldi r16, 'A'
	ldi r17, 3
	call show_char
	ldi r16, 'R'
	ldi r17, 4
	call show_char
	ldi r16, 'R'
	ldi r17, 5
	call show_char
	ldi r16, 'Y'
	ldi r17, 6
	call show_char
	jmp end

zero:
	ldi r16, '0'
	ldi r17, 2
	call show_char
	jmp end
