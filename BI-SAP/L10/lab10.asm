; definice pro nas typ procesoru
.include "m169def.inc"
; podprogramy pro praci s displejem
.org 0x1000
.include "print.inc"

; Zacatek programu - po resetu
.org 0
jmp main

; Zacatek programu - hlavni program
.org 0x100

main:
    ; Inicializace zasobniku
	ldi r16, 0xFF
	out SPL, r16
	ldi r16, 0x04
	out SPH, r16
    ; Inicializace displeje
	call init_disp
	call init_joy
	call init_forbes
	call start
	call first
	call second
	call third
	call done


init_forbes:
	call clear
	; registry se zobrazovanymi znaky
	ldi r21, 'A'
	ldi r22, 'B'
	ldi r23, 'C'
	ret

start:
	ldi r16, 'S'
	ldi r17, 2
	call show_char
	
	ldi r16, 'T'
	ldi r17, 3
	call show_char

	ldi r16, 'A'
	ldi r17, 4
	call show_char

	ldi r16, 'R'
	ldi r17, 5
	call show_char

	ldi r16, 'T'
	ldi r17, 6
	call show_char

	call wait ; aby se obrazovka zobrazila po opakovanem hrani
	
	; loop dokud nebyl zmacknuty enter, 
	call read_joy
	cpi r20, 1
	brne start

	; pocka do konce zmacknuti tlacitka
	call endpress

	; jde se dal
	call clear
	ret


first:
	; posunuti znaku
	inc r21
	inc r22
	inc r23
	call modulo

	; vykresleni aktualnich znaku
	mov r16, r21
	ldi r17, 2
	call show_char
	
	mov r16, r22
	ldi r17, 3
	call show_char

	mov r16, r23
	ldi r17, 4
	call show_char

	; dokud nebyl zmackuty enter, vykresluji se znaky
	call wait
	call read_joy
	cpi r20, 1
	brne first

	; pocka do konce zmacknuti tlacitka
	call endpress

	ret

second:
	; posunuti znaku
	inc r21
	inc r22
	call modulo

	; vykresleni znaku
	mov r16, r21
	ldi r17, 2
	call show_char
	
	mov r16, r22
	ldi r17, 3
	call show_char

	; dokud nebyl zmackuty enter, vykresluji se znaky
	call wait
	call read_joy
	cpi r20, 1
	brne second

	; pocka do konce zmacknuti tlacitka
	call endpress

	ret


third:
	; posunuti znaku
	inc r21
	call modulo

	; vykresleni znaku
	mov r16, r21
	ldi r17, 2
	call show_char

	; dokud nebyl zmackuty enter, vykresluji se znaky
	call wait
	call read_joy
	cpi r20, 1
	brne third

	; pocka do konce zmacknuti tlacitka
	call endpress

	ret


done:
	call wait
	ldi r24, 3 ; zablika se 3x
	sub r21, r23
	brne lose ; prohra pokud znak1 != znak3
	sub r22, r23
	brne lose ; prohra pokud znak1 != znak3
	jmp win ; jinak vyhra


lose:
	ldi r16, 'L'
	ldi r17, 2
	call show_char
	
	ldi r16, 'O'
	ldi r17, 3
	call show_char

	ldi r16, 'S'
	ldi r17, 4
	call show_char

	ldi r16, 'E'
	ldi r17, 5
	call show_char

	ldi r16, 'R'
	ldi r17, 6
	call show_char

	call wait

	; po trech zablikani se vyskoci ven
	dec r24
	breq end

	; clear obrazovky pro efekt blikani
	call clear
	call wait
	jmp lose


win:
	ldi r16, 'W'
	ldi r17, 2
	call show_char
	
	ldi r16, 'I'
	ldi r17, 3
	call show_char

	ldi r16, 'N'
	ldi r17, 4
	call show_char

	ldi r16, 'N'
	ldi r17, 5
	call show_char

	ldi r16, 'E'
	ldi r17, 6
	call show_char

	ldi r16, 'R'
	ldi r17, 7
	call show_char

	call wait

	; po trech zablikani se vyskoci ven
	dec r24
	breq end

	; clear obrazovky pro efekt blikani
	call clear
	call wait
	jmp win


; zmoduluje znaky, aktualne modulo D
modulo:
	SBRC r21, 2
	ldi r21, 'A'

	SBRC r22, 2
	ldi r22, 'A'

	SBRC r23, 2
	ldi r23, 'A'
	ret


; pocka do konce zmacknuti tlacitka
endpress:
	call read_joy
	cpi r20, 1
	breq endpress
	ret


; ceka na enter, pak spusti cely program od znova
end: 
	call read_joy
	cpi r20, 1
	brne end

	; pocka do konce zmacknuti tlacitka
	call endpress

	jmp main


; vycisti obrazovku
clear:
	ldi r16, ' '
	ldi r17, 2
	call show_char
	
	ldi r16, ' '
	ldi r17, 3
	call show_char

	ldi r16, ' '
	ldi r17, 4
	call show_char

	ldi r16, ' '
	ldi r17, 5
	call show_char

	ldi r16, ' '
	ldi r17, 6
	call show_char

	ldi r16, ' '
	ldi r17, 7
	call show_char
	ret


wait:
	; doba cekani je nastavena pro simulaci
	ldi r25, 12
	cek3:
	    ldi r26, 100
		cek2:
			ldi r27, 100
			cek:
				dec r27
	    		brne cek
	    dec r26
	    brne cek2
	dec r25
	brne cek3


; ukradeno z courses
init_joy:                ; inicializace joysticku
    in r17, DDRE
    andi r17, 0b11110011
    in r16, PORTE
    ori r16, 0b00001100
    out DDRE, r17
    out PORTE, r16
    ldi r16, 0b00000000
    sts DIDR1, r16
    in r17, DDRB
    andi r17, 0b00101111
    in r16, PORTB
    ori r16, 0b11010000
    out DDRB, r17
    out PORTB, r16
	ret

read_joy:                ; ulozi smer joysticku do registru r20
    push r16             ; uklid r16 a r17
    push r17

joy_reread:
    in r16, PINB         ; nacti hodnotu joysticku 1

    ldi r20, 255         ; chvili cekej 2
joy_wait: dec r20
    brne joy_wait

    in r17, PINB         ; nacti jeste jednou

    andi r16, 0b00010000 ; vymaskuj ostatni bity 3
    andi r17, 0b00010000

    cp r16, r17
    brne joy_reread      ; hodnoty se nerovnaly -> nacti znovu

    ldi r20, 0           ; vychozi hodnota - nic neni aktivni
    cpi r16, 0
    brne joy_no_enter    ; hodnota je inverzni -> neni 0 znamena neni aktivni 4
    ldi r20, 1           ; r20 = 1, kdyz je enter aktivni
joy_no_enter:

   	pop r17               ; obnoveni r16 a r17
   	pop r16
	ret
