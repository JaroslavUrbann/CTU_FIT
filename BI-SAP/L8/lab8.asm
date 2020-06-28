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

    ; R20 = (4 * R16 + 3 * R17 - R18) / 8
	ldi r16, 5
	ldi r17, 10
	ldi r18, 58

	; 3 * r17
	mov r20, r17
	lsl r17
	add r20, r17

	; 4 * r16
	lsl r16
	lsl r16
	mov r20, r16

	; - r18
	sub r20, r18

	; / 8
	asr r20
	asr r20
	asr r20


end: jmp end        ; Zastavime program - nekonecna smycka
