.include "m328pdef.inc"
.def flag=R16


init:
	ldi R19, $3F    ;настройка на вывод
	out DDRB, R19   ;настройка на вывод линий 0..5 портаа B
	ldi zh, HIGH(array*2)
main:
	ldi zl, LOW(array*2)
	ldi flag,0x04   ;счётчик на 4 комбинации ШД

loop:
	lpm R19, z+     ;взять из памяти программ число<
	out PORTB, R19
	rcall Delay     ;вызов подпрограммы задержки
	dec flag        ;уменьшить счётчик на 1
	brne loop       ;если Z = 0 переход на loop
	rjmp main       ;делаем следующий оборот

Delay:
	ldi R19, 0xFC   ;252
	ldi R20, 0x69   ;105
	ldi R21, 0x18   ;24
m1: 	                ;сидим в этих циклах 0,5с.
	subi R19 ,1     ;вычитание 1 из регистра r19
	sbci R20,0      ;вычитание только С
	sbci R21 ,0     ;вычитание только С
	brcc m1         ;если не было заёма вернуться к метке Delay
        ret

array: .db 0b00100100, 0b00001010, 0b00010100, 0b00000011
