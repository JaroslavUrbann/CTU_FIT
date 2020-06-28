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

    ; r25:r24 = (4 * R16 + 3 * R17 - R18) / 8
	;----------------------------------------------------------------
	ldi r16, 127
	ldi r17, 127
	ldi r18, 0
	;----------------------------------------------------------------

; prepísu r17 do r25:r24
	; výsledek bude v r25,r24
	mov r24, r17
	clr r25

	; pokud je r17 záporné, vyplní r25 jednickami
	SBRC r17, 7
	ldi r25, 0b11111111
	; pokud byl práve vyplnen jednickami, nevyplní se nulami
	SBRS r17, 7
	ldi r25, 0b00000000

; bitový posun pres 2 registry
	; 2 * r17
	lsl r24
	rol r25

; r17 se rozsiri o r26 na r26:r17
	clr r26 ; pomocný registr
	; pokud je r17 záporné, vyplní r26 jednickami
	SBRC r17, 7
	ldi r26, 0b11111111
	; pokud byl práve vyplnen jednickami, nevyplní se nulami
	SBRS r17, 7
	ldi r26, 0b00000000

	; + r17
	add r24, r17
	adc r25, r26

	; pokud je vysledek souctu zaporny, tak se zneguje a pricte se k nemu 1
	BRVC neni_zaporne
	clr r27
	neg r24
	neg r25
	subi r24, -1
	adc r25, r27

neni_zaporne:

; r16 se rozsiri na r27:r16
	; pokud je r16 záporné, vyplní r27 jednickami
	SBRC r16, 7
	ldi r27, 0b11111111
	; pokud byl práve vyplnen jednickami, nevyplní se nulami
	SBRS r16, 7
	ldi r27, 0b00000000

; bitový posun a následný soucet s dosavadním výsledkem jako u r17
	; 4 * r16
	lsl r16
	rol r27
	lsl r16
	rol r27

	; + 4 * r16
	add r24, r16
	adc r25, r27

	; zneguje a pricte jednicku, pokud je vysledek záporný
	BRVC neni_zaporne2
	clr r27
	neg r24
	neg r25
	subi r24, -1
	adc r25, r27

neni_zaporne2:

	; - R18
	neg r18

; r18 se rozsiri na r27:r18
	; pokud je r18 záporné, vyplní r27 jednickami
	SBRC r18, 7
	ldi r27, 0b11111111
	; pokud byl práve vyplnen jednickami, nevyplní se nulami
	SBRS r18, 7
	ldi r27, 0b00000000

; soucet s vysledkem
	add r24, r18
	adc r25, r27

	; zneguje a pricte jednicku, pokud vysledek záporný
	BRVC neni_zaporne3
	clr r27
	neg r24
	neg r25
	subi r24, -1
	adc r25, r27

neni_zaporne3:

	clr r27 ; r27 > 0, pokud dojde ke ztrate presnosti
	; / 2
	asr r25
	ror r24
	adc r27, r27 ; pokud c > 0, vznikla ztrata na presnosti => r27 > 0
	; / 2
	asr r25
	ror r24
	adc r27, r27
	; / 2
	asr r25
	ror r24
	adc r27, r27

	; preskocí zobrazení P, pokud nedoslo ke ztráte presnosti
	clr r20
	cpse r27, r20 ; preskocí dalsi instrukci, pokud r27 == 0
	call draw_P

; vykresli vysledek
	mov r18, r24
	ldi r22, 5
	ldi r23, 4
	call show_num_16
	mov r18, r25
	ldi r22, 3
	ldi r23, 2
	call show_num_16	

end: jmp end        ; Zastavime program - nekonecna smycka

; zobrazení P
draw_P:
	ldi r16, 'P'
	ldi r17, 6
	call show_char
	ret

; funkce z lab8_2, zobrazí cislo z r18 na pozice r22, r23 v sestnactkove soustave
show_num_16:

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
	mov r17, r22
	call show_char

	mov r16, r20
	mov r17, r23
	call show_char
	ret
