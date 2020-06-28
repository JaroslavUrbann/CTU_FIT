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

    ; ----------------------------------------------------------
	ldi r18, 15				; zadání císla
	; ----------------------------------------------------------

	mov r19, r18
	mov r20, r18

	; získání horního bytu ctyrmi posuny 
	ldi r21, 4
	get_higher_byte:
		lsr r20
		dec r21
		brne get_higher_byte
		
	; získání spodního bytu maskou
	andi r19, 0b00001111

	; pricte 'A', pokud je cislo >= 10, jinak pricte '0'
	subi r19, -'0'
	cpi r19, '0'+10
	brmi dontaddA19
	subi r19, -('A'-'0'-10)
dontaddA19:
	subi r20, -'0'
	cpi r20, '0'+10
	brmi dontaddA20
	subi r20, -('A'-'0'-10)
dontaddA20:
	; zobrazí znaky
	mov r16, r19
	ldi r17, 3
	call show_char

	mov r16, r20
	ldi r17, 2
	call show_char

end: jmp end        ; Zastavime program - nekonecna smycka

