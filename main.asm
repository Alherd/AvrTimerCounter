.include "m328pdef.inc"
.def flag=R16


init:
	ldi R19, $3F    ;��������� �� �����
	out DDRB, R19   ;��������� �� ����� ����� 0..5 ������ B
	ldi zh, HIGH(array*2)
main:
	ldi zl, LOW(array*2)
	ldi flag,0x04   ;������� �� 4 ���������� ��

loop:
	lpm R19, z+     ;����� �� ������ �������� �����<
	out PORTB, R19
	rcall Delay     ;����� ������������ ��������
	dec flag        ;��������� ������� �� 1
	brne loop       ;���� Z = 0 ������� �� loop
	rjmp main       ;������ ��������� ������

Delay:
	ldi R19, 0xFC   ;252
	ldi R20, 0x69   ;105
	ldi R21, 0x18   ;24
m1: 	                ;����� � ���� ������ 0,5�.
	subi R19 ,1     ;��������� 1 �� �������� r19
	sbci R20,0      ;��������� ������ �
	sbci R21 ,0     ;��������� ������ �
	brcc m1         ;���� �� ���� ���� ��������� � ����� Delay
        ret

array: .db 0b00100100, 0b00001010, 0b00010100, 0b00000011
