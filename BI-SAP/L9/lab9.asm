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
str: .db 0,0,0,0,0,0,0,0,0,0,"ABCDE FGHIJK LMNOP",0,0,0,0,0,0,0,0,0,0	; zad�n� stringu
start:
    ; Inicializace zasobniku
	ldi r16, 0xFF
	out SPL, r16
	ldi r16, 0x04
	out SPH, r16
    ; Inicializace displeje
	call init_disp
	ldi r18, 4					; zad�n� d�lky displeje
	clr r21 ; (bool) objevilo se uz na prvn� pozici displeje (zleva) p�smeno?
	ldi r20, 10 ; maxim�ln� s�rka displeje
	sub r20, r18 ; posun zac�tku pameti (10 - sirka displeje)
	ldi r28, low(2*str) ; nacten� zac�tku pameti
	ldi r29, high(2*str)
	add r28, r20 ; posunut� zac�tku pameti
	adc r29, r21
	jmp move

end: jmp end        ; Zastavime program - nekonecna smycka

; zobraz� okno o velikosti r18, kter� zac�n� v pameti na adrese v registru Y
move:
	; nacten� adresy do registru Z
	mov r30, r28
	mov r31, r29

	; pokud adresa ukazuje na nulu, zkontroluje, jestli se uz nedojelo na konec
	lpm r22, Z
	subi r22, 0
	breq did_anim_end

	ldi r21, 1 ; na prvn� pozici se objevilo p�smeno -> r21 = 1
	jmp display_window

; pokud je na prvn� pozici okna 0 a uz na n� bylo p�smeno, pojede se odznova
did_anim_end:
	subi r21, 0
	brne start

; vykresli okno sirky r18
display_window:
	mov r22, r18
	ldi r17, 2 ; displej zacina na pozici 2
	display_char:
		lpm r16, Z+
		subi r16, 0
		brne show
		ldi r16, ' ' ; zobraz� mezeru, pokud je na adrese nula
		show:
		call show_char
		inc r17
		dec r22
		brne display_char

	; 3*50*100*100/(2*10^6)=0.75s
	/*
	ldi r23, 50
	cek3:
	    ldi r24, 100
		cek2:
			ldi r25, 100
			cek:
				dec r25
	    		brne cek
	    dec r24
	    brne cek2
	dec r23
	brne cek3
	*/

	; +1 registr Y, zacatek okna se posune doprava
	clr r23
	inc r28
	adc r29, r23

	jmp move
