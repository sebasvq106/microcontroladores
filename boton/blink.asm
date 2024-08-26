;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (Linux)
;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.file	"boton.c"
	list	p=12f675
	radix dec
	include "p12f675.inc"
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	__modsint
	extern	_TRISIO
	extern	_GPIO
	extern	_GPIObits
	extern	__gptrget1
	extern	__sdcc_gsinit_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_main
	global	_duplicate
	global	_shiftOutTwo
	global	_delay
	global	_segment_map

	global PSAVE
	global SSAVE
	global WSAVE
	global STK12
	global STK11
	global STK10
	global STK09
	global STK08
	global STK07
	global STK06
	global STK05
	global STK04
	global STK03
	global STK02
	global STK01
	global STK00

sharebank udata_ovr 0x0020
PSAVE	res 1
SSAVE	res 1
WSAVE	res 1
STK12	res 1
STK11	res 1
STK10	res 1
STK09	res 1
STK08	res 1
STK07	res 1
STK06	res 1
STK05	res 1
STK04	res 1
STK03	res 1
STK02	res 1
STK01	res 1
STK00	res 1

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
UDL_boton_0	udata
r0x1015	res	1
r0x1014	res	1
r0x1016	res	1
r0x1017	res	1
r0x1018	res	1
r0x1019	res	1
r0x101A	res	1
r0x101B	res	1
r0x101C	res	1
r0x101D	res	1
r0x101E	res	1
r0x101F	res	1
r0x1020	res	1
r0x1021	res	1
r0x1022	res	1
r0x1023	res	1
r0x1024	res	1
r0x1025	res	1
r0x1026	res	1
r0x1027	res	1
r0x1029	res	1
r0x1028	res	1
_main_r1_values_65537_18	res	10
_main_r2_values_65537_18	res	10
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------

IDC_boton_0	code
_segment_map
	retlw 0x7e	; 126
	retlw 0x0c	; 12
	retlw 0xb6	; 182
	retlw 0x9e	; 158
	retlw 0xcc	; 204
	retlw 0xda	; 218
	retlw 0xfa	; 250
	retlw 0x0e	; 14
	retlw 0xff	; 255
	retlw 0xce	; 206
;--------------------------------------------------------
; initialized absolute data
;--------------------------------------------------------
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; reset vector 
;--------------------------------------------------------
STARTUP	code 0x0000
	nop
	pagesel __sdcc_gsinit_startup
	goto	__sdcc_gsinit_startup
;--------------------------------------------------------
; code
;--------------------------------------------------------
code_boton	code
;***
;  pBlock Stats: dbName = M
;***
;has an exit
;functions called:
;   __modsint
;   __modsint
;   _delay
;   _duplicate
;   __modsint
;   __modsint
;   __gptrget1
;   __gptrget1
;   _shiftOutTwo
;   _delay
;   _delay
;   __gptrget1
;   _shiftOutTwo
;   _delay
;   _shiftOutTwo
;   __modsint
;   __modsint
;   _delay
;   _duplicate
;   __modsint
;   __modsint
;   __gptrget1
;   __gptrget1
;   _shiftOutTwo
;   _delay
;   _delay
;   __gptrget1
;   _shiftOutTwo
;   _delay
;   _shiftOutTwo
;17 compiler assigned registers:
;   r0x1022
;   r0x1023
;   r0x1024
;   r0x1025
;   r0x1026
;   STK02
;   STK01
;   STK00
;   r0x1027
;   r0x1028
;   r0x1029
;   r0x102A
;   STK07
;   STK06
;   STK05
;   STK04
;   STK03
;; Starting pCode block
S_boton__main	code
_main:
; 2 exit points
;	.line	63; "boton.c"	TRISIO = 0b00100000; // Todos los pines como salida excepto GP5 que es entrada
	MOVLW	0x20
	BANKSEL	_TRISIO
	MOVWF	_TRISIO
;	.line	64; "boton.c"	GPIO = 0x00; // Inicializar todos los pines en bajo
	BANKSEL	_GPIO
	CLRF	_GPIO
;	.line	66; "boton.c"	unsigned char r1 = 0; // Contador para el primer número
	CLRF	r0x1022
;	.line	67; "boton.c"	unsigned char r2 = 0; // Contador para el segundo número
	CLRF	r0x1023
;	.line	70; "boton.c"	unsigned char index = 0; // Contador de pulsaciones del botón
	CLRF	r0x1024
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
;	.line	71; "boton.c"	unsigned char r1_values[10] = {0}; // Array para guardar los valores de r1
	CLRF	(_main_r1_values_65537_18 + 0)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r1_values_65537_18 + 1)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r1_values_65537_18 + 2)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r1_values_65537_18 + 3)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r1_values_65537_18 + 4)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r1_values_65537_18 + 5)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r1_values_65537_18 + 6)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r1_values_65537_18 + 7)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r1_values_65537_18 + 8)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r1_values_65537_18 + 9)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
;	.line	72; "boton.c"	unsigned char r2_values[10] = {0}; // Array para guardar los valores de r2
	CLRF	(_main_r2_values_65537_18 + 0)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r2_values_65537_18 + 1)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r2_values_65537_18 + 2)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r2_values_65537_18 + 3)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r2_values_65537_18 + 4)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r2_values_65537_18 + 5)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r2_values_65537_18 + 6)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r2_values_65537_18 + 7)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r2_values_65537_18 + 8)
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	CLRF	(_main_r2_values_65537_18 + 9)
_00201_DS_:
;	.line	76; "boton.c"	if (push_button == 0) {
	BANKSEL	_GPIObits
	BTFSC	_GPIObits,5
	GOTO	_00187_DS_
;	.line	77; "boton.c"	r1 = (r1 + speed1) % 10; // Incremento rápido y ciclado entre 0 y 9
	MOVF	r0x1022,W
	MOVWF	r0x1025
	CLRF	r0x1026
	INCF	r0x1025,F
	BTFSC	STATUS,2
	INCF	r0x1026,F
	MOVLW	0x0a
	MOVWF	STK02
	MOVLW	0x00
	MOVWF	STK01
	MOVF	r0x1025,W
	MOVWF	STK00
	MOVF	r0x1026,W
	PAGESEL	__modsint
	CALL	__modsint
	PAGESEL	$
	MOVWF	r0x1026
	MOVF	STK00,W
	MOVWF	r0x1025
	MOVWF	r0x1022
;	.line	78; "boton.c"	r2 = (r2 + speed2) % 10;
	MOVF	r0x1023,W
	MOVWF	r0x1025
	CLRF	r0x1026
	MOVLW	0x03
	ADDWF	r0x1025,F
	BTFSC	STATUS,0
	INCF	r0x1026,F
	MOVLW	0x0a
	MOVWF	STK02
	MOVLW	0x00
	MOVWF	STK01
	MOVF	r0x1025,W
	MOVWF	STK00
	MOVF	r0x1026,W
	PAGESEL	__modsint
	CALL	__modsint
	PAGESEL	$
	MOVWF	r0x1026
	MOVF	STK00,W
	MOVWF	r0x1025
	MOVWF	r0x1023
;	.line	79; "boton.c"	delay(10); // Pequeño retardo para que los números cambien rápidamente
	MOVLW	0x0a
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
_00187_DS_:
;	.line	82; "boton.c"	if (push_button == 1) { 
	CLRF	r0x1025
	BANKSEL	_GPIObits
	BTFSC	_GPIObits,5
	INCF	r0x1025,F
	MOVF	r0x1025,W
	XORLW	0x01
	BTFSS	STATUS,2
	GOTO	_00195_DS_
_00188_DS_:
;	.line	84; "boton.c"	while (duplicate(r1, r2, r1_values, r2_values, index)) {
	MOVLW	high (_main_r1_values_65537_18 + 0)
	MOVWF	r0x1026
	MOVLW	(_main_r1_values_65537_18 + 0)
	MOVWF	r0x1025
	CLRF	r0x1027
	MOVLW	high (_main_r2_values_65537_18 + 0)
	MOVWF	r0x1028
	MOVLW	(_main_r2_values_65537_18 + 0)
	MOVWF	r0x1029
;;1	CLRF	r0x102A
	MOVF	r0x1024,W
	MOVWF	STK07
	MOVF	r0x1029,W
	MOVWF	STK06
	MOVF	r0x1028,W
	MOVWF	STK05
	MOVLW	0x00
	MOVWF	STK04
	MOVF	r0x1025,W
	MOVWF	STK03
	MOVF	r0x1026,W
	MOVWF	STK02
	MOVLW	0x00
	MOVWF	STK01
	MOVF	r0x1023,W
	MOVWF	STK00
	MOVF	r0x1022,W
	PAGESEL	_duplicate
	CALL	_duplicate
	PAGESEL	$
	MOVWF	r0x1025
	MOVF	r0x1025,W
	BTFSC	STATUS,2
	GOTO	_00190_DS_
;	.line	86; "boton.c"	r1 = (r1 + speed1) % 10;
	MOVF	r0x1022,W
	MOVWF	r0x1025
	CLRF	r0x1026
	INCF	r0x1025,F
	BTFSC	STATUS,2
	INCF	r0x1026,F
	MOVLW	0x0a
	MOVWF	STK02
	MOVLW	0x00
	MOVWF	STK01
	MOVF	r0x1025,W
	MOVWF	STK00
	MOVF	r0x1026,W
	PAGESEL	__modsint
	CALL	__modsint
	PAGESEL	$
	MOVWF	r0x1026
	MOVF	STK00,W
	MOVWF	r0x1025
	MOVWF	r0x1022
;	.line	87; "boton.c"	r2 = (r2 + speed2) % 10;
	MOVF	r0x1023,W
	MOVWF	r0x1025
	CLRF	r0x1026
	MOVLW	0x03
	ADDWF	r0x1025,F
	BTFSC	STATUS,0
	INCF	r0x1026,F
	MOVLW	0x0a
	MOVWF	STK02
	MOVLW	0x00
	MOVWF	STK01
	MOVF	r0x1025,W
	MOVWF	STK00
	MOVF	r0x1026,W
	PAGESEL	__modsint
	CALL	__modsint
	PAGESEL	$
	MOVWF	r0x1026
	MOVF	STK00,W
	MOVWF	r0x1025
	MOVWF	r0x1023
	GOTO	_00188_DS_
_00190_DS_:
;	.line	90; "boton.c"	r1_values[index] = r1;
	MOVF	r0x1024,W
	ADDLW	(_main_r1_values_65537_18 + 0)
	MOVWF	r0x1025
	MOVLW	high (_main_r1_values_65537_18 + 0)
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x1026
	MOVF	r0x1025,W
	MOVWF	FSR
	BCF	STATUS,7
	BTFSC	r0x1026,0
	BSF	STATUS,7
	MOVF	r0x1022,W
	MOVWF	INDF
;	.line	91; "boton.c"	r2_values[index] = r2;
	MOVF	r0x1024,W
	ADDLW	(_main_r2_values_65537_18 + 0)
	MOVWF	r0x1025
	MOVLW	high (_main_r2_values_65537_18 + 0)
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x1026
	MOVF	r0x1025,W
	MOVWF	FSR
	BCF	STATUS,7
	BTFSC	r0x1026,0
	BSF	STATUS,7
	MOVF	r0x1023,W
	MOVWF	INDF
;	.line	92; "boton.c"	shiftOutTwo(segment_map[r1], segment_map[r2]); // Enviar los valores a los 74HC595
	MOVF	r0x1022,W
	ADDLW	(_segment_map + 0)
	MOVWF	r0x1025
	MOVLW	high (_segment_map + 0)
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x1026
	MOVF	r0x1025,W
	MOVWF	STK01
	MOVF	r0x1026,W
	MOVWF	STK00
	MOVLW	0x80
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	MOVWF	r0x1027
	MOVF	r0x1023,W
	ADDLW	(_segment_map + 0)
	MOVWF	r0x1025
	MOVLW	high (_segment_map + 0)
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x1026
	MOVF	r0x1025,W
	MOVWF	STK01
	MOVF	r0x1026,W
	MOVWF	STK00
	MOVLW	0x80
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	MOVWF	r0x1029
	MOVWF	STK00
	MOVF	r0x1027,W
	PAGESEL	_shiftOutTwo
	CALL	_shiftOutTwo
	PAGESEL	$
;	.line	93; "boton.c"	index++;
	INCF	r0x1024,F
_00191_DS_:
;	.line	96; "boton.c"	while (push_button == 1);
	CLRF	r0x1025
	BANKSEL	_GPIObits
	BTFSC	_GPIObits,5
	INCF	r0x1025,F
	MOVF	r0x1025,W
;	.line	99; "boton.c"	delay(100);
	XORLW	0x01
	BTFSC	STATUS,2
	GOTO	_00191_DS_
	MOVLW	0x64
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;;unsigned compare: left < lit(0xA=10), size=1
_00195_DS_:
;	.line	103; "boton.c"	if (index >= 10) {
	MOVLW	0x0a
	SUBWF	r0x1024,W
	BTFSS	STATUS,0
	GOTO	_00201_DS_
;;genSkipc:3307: created from rifx:0x7ffece0fba20
;	.line	104; "boton.c"	for (unsigned char blink = 0; blink < 3; blink++) {
	CLRF	r0x1025
;;unsigned compare: left < lit(0x3=3), size=1
_00204_DS_:
	MOVLW	0x03
	SUBWF	r0x1025,W
	BTFSC	STATUS,0
	GOTO	_00196_DS_
;;genSkipc:3307: created from rifx:0x7ffece0fba20
;	.line	105; "boton.c"	delay(250); // Pausa
	MOVLW	0xfa
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	106; "boton.c"	shiftOutTwo(segment_map[9], segment_map[9]); // Mostrar 99
	MOVLW	(_segment_map + 9)
	MOVWF	STK01
	MOVLW	high (_segment_map + 9)
	MOVWF	STK00
	MOVLW	0x80
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	MOVWF	r0x1026
	MOVWF	STK00
	MOVF	r0x1026,W
	PAGESEL	_shiftOutTwo
	CALL	_shiftOutTwo
	PAGESEL	$
;	.line	107; "boton.c"	delay(250); // Pausa
	MOVLW	0xfa
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	108; "boton.c"	shiftOutTwo(0x00, 0x00); // Apagar display
	MOVLW	0x00
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_shiftOutTwo
	CALL	_shiftOutTwo
	PAGESEL	$
;	.line	104; "boton.c"	for (unsigned char blink = 0; blink < 3; blink++) {
	INCF	r0x1025,F
	GOTO	_00204_DS_
_00196_DS_:
;	.line	111; "boton.c"	index = 0; // Reiniciar el contador para los próximos números
	CLRF	r0x1024
;	.line	112; "boton.c"	for (unsigned char i = 0; i < 10; i++) {
	CLRF	r0x1025
;;unsigned compare: left < lit(0xA=10), size=1
_00207_DS_:
	MOVLW	0x0a
	SUBWF	r0x1025,W
	BTFSC	STATUS,0
	GOTO	_00201_DS_
;;genSkipc:3307: created from rifx:0x7ffece0fba20
;	.line	113; "boton.c"	r1_values[i] = 0; // Reiniciar el array de valores de r1
	MOVF	r0x1025,W
	ADDLW	(_main_r1_values_65537_18 + 0)
	MOVWF	r0x1026
	MOVLW	high (_main_r1_values_65537_18 + 0)
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x1027
	MOVF	r0x1026,W
	MOVWF	FSR
	BCF	STATUS,7
	BTFSC	r0x1027,0
	BSF	STATUS,7
	MOVLW	0x00
	MOVWF	INDF
;	.line	114; "boton.c"	r2_values[i] = 0; // Reiniciar el array de valores de r2
	MOVF	r0x1025,W
	ADDLW	(_main_r2_values_65537_18 + 0)
	MOVWF	r0x1026
	MOVLW	high (_main_r2_values_65537_18 + 0)
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x1027
	MOVF	r0x1026,W
	MOVWF	FSR
	BCF	STATUS,7
	BTFSC	r0x1027,0
	BSF	STATUS,7
	MOVLW	0x00
	MOVWF	INDF
;	.line	112; "boton.c"	for (unsigned char i = 0; i < 10; i++) {
	INCF	r0x1025,F
	GOTO	_00207_DS_
;	.line	118; "boton.c"	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   __gptrget1
;   __gptrget1
;   __gptrget1
;   __gptrget1
;22 compiler assigned registers:
;   r0x1014
;   STK00
;   r0x1015
;   STK01
;   r0x1016
;   STK02
;   r0x1017
;   STK03
;   r0x1018
;   STK04
;   r0x1019
;   STK05
;   r0x101A
;   STK06
;   r0x101B
;   STK07
;   r0x101C
;   r0x101D
;   r0x101E
;   r0x101F
;   r0x1020
;   r0x1021
;; Starting pCode block
S_boton__duplicate	code
_duplicate:
; 2 exit points
;	.line	53; "boton.c"	unsigned char duplicate(unsigned char r1, unsigned char r2, unsigned char *r1_values, unsigned char *r2_values, unsigned char size) {
	MOVWF	r0x1014
	MOVF	STK00,W
	MOVWF	r0x1015
	MOVF	STK01,W
	MOVWF	r0x1016
	MOVF	STK02,W
	MOVWF	r0x1017
	MOVF	STK03,W
	MOVWF	r0x1018
	MOVF	STK04,W
	MOVWF	r0x1019
	MOVF	STK05,W
	MOVWF	r0x101A
	MOVF	STK06,W
	MOVWF	r0x101B
	MOVF	STK07,W
	MOVWF	r0x101C
;	.line	54; "boton.c"	for (unsigned char i = 0; i < size; i++) {
	CLRF	r0x101D
_00179_DS_:
	MOVF	r0x101C,W
	SUBWF	r0x101D,W
	BTFSC	STATUS,0
	GOTO	_00177_DS_
;;genSkipc:3307: created from rifx:0x7ffece0fba20
;	.line	55; "boton.c"	if (r1_values[i] == r1 && r2_values[i] == r2) {
	MOVF	r0x101D,W
	ADDWF	r0x1018,W
	MOVWF	r0x101E
	MOVF	r0x1017,W
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x101F
;;102	MOVF	r0x1016,W
	MOVF	r0x101E,W
	MOVWF	STK01
	MOVF	r0x101F,W
	MOVWF	STK00
;;101	MOVF	r0x1020,W
	MOVF	r0x1016,W
	MOVWF	r0x1020
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	MOVWF	r0x1021
	XORWF	r0x1014,W
	BTFSS	STATUS,2
	GOTO	_00180_DS_
	MOVF	r0x101D,W
	ADDWF	r0x101B,W
	MOVWF	r0x101E
	MOVF	r0x101A,W
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x101F
;;100	MOVF	r0x1019,W
	MOVF	r0x101E,W
	MOVWF	STK01
	MOVF	r0x101F,W
	MOVWF	STK00
;;99	MOVF	r0x1020,W
	MOVF	r0x1019,W
	MOVWF	r0x1020
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	MOVWF	r0x1021
	XORWF	r0x1015,W
	BTFSS	STATUS,2
	GOTO	_00180_DS_
;	.line	56; "boton.c"	return 1; // Ambos valores coinciden en el mismo índice
	MOVLW	0x01
	GOTO	_00181_DS_
_00180_DS_:
;	.line	54; "boton.c"	for (unsigned char i = 0; i < size; i++) {
	INCF	r0x101D,F
	GOTO	_00179_DS_
_00177_DS_:
;	.line	59; "boton.c"	return 0; // No hay duplicados
	MOVLW	0x00
_00181_DS_:
;	.line	60; "boton.c"	}
	RETURN	
; exit point of _duplicate

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;5 compiler assigned registers:
;   r0x1018
;   STK00
;   r0x1019
;   r0x101A
;   r0x101B
;; Starting pCode block
S_boton__shiftOutTwo	code
_shiftOutTwo:
; 2 exit points
;	.line	30; "boton.c"	void shiftOutTwo(unsigned char data1, unsigned char data2) {
	MOVWF	r0x1018
	MOVF	STK00,W
	MOVWF	r0x1019
;	.line	31; "boton.c"	for (unsigned char i = 7; i < 0xFF; i--) { // Usar < 0xFF para evitar problemas de comparación
	MOVLW	0x07
	MOVWF	r0x101A
;;unsigned compare: left < lit(0xFF=255), size=1
_00139_DS_:
	MOVLW	0xff
	SUBWF	r0x101A,W
	BTFSC	STATUS,0
	GOTO	_00136_DS_
;;genSkipc:3307: created from rifx:0x7ffece0fba20
;	.line	32; "boton.c"	DS = (data1 >> i) & 0x01; // Enviar cada bit del primer número
	MOVF	r0x1018,W
	MOVWF	r0x101B
	MOVF	r0x101A,W
	SUBLW	0x00
	BTFSC	STATUS,2
	GOTO	_00166_DS_
_00165_DS_:
	RRF	r0x101B,F
	ADDLW	0x01
	BTFSS	STATUS,0
	GOTO	_00165_DS_
_00166_DS_:
	RRF	r0x101B,W
	BTFSC	STATUS,0
	GOTO	_00001_DS_
	BANKSEL	_GPIObits
	BCF	_GPIObits,0
_00001_DS_:
	BTFSS	STATUS,0
	GOTO	_00002_DS_
	BANKSEL	_GPIObits
	BSF	_GPIObits,0
_00002_DS_:
;	.line	33; "boton.c"	SHC = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,2
;	.line	34; "boton.c"	delay(5);
	MOVLW	0x05
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	35; "boton.c"	SHC = 0;
	BANKSEL	_GPIObits
	BCF	_GPIObits,2
;	.line	36; "boton.c"	delay(5);
	MOVLW	0x05
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	31; "boton.c"	for (unsigned char i = 7; i < 0xFF; i--) { // Usar < 0xFF para evitar problemas de comparación
	DECF	r0x101A,F
	GOTO	_00139_DS_
_00136_DS_:
;	.line	39; "boton.c"	for (unsigned char i = 7; i < 0xFF; i--) { // Usar < 0xFF para evitar problemas de comparación
	MOVLW	0x07
	MOVWF	r0x1018
;;unsigned compare: left < lit(0xFF=255), size=1
_00142_DS_:
	MOVLW	0xff
	SUBWF	r0x1018,W
	BTFSC	STATUS,0
	GOTO	_00137_DS_
;;genSkipc:3307: created from rifx:0x7ffece0fba20
;	.line	40; "boton.c"	DS = (data2 >> i) & 0x01; // Enviar cada bit del segundo número
	MOVF	r0x1019,W
	MOVWF	r0x101A
	MOVF	r0x1018,W
	SUBLW	0x00
	BTFSC	STATUS,2
	GOTO	_00169_DS_
_00168_DS_:
	RRF	r0x101A,F
	ADDLW	0x01
	BTFSS	STATUS,0
	GOTO	_00168_DS_
_00169_DS_:
	RRF	r0x101A,W
	BTFSC	STATUS,0
	GOTO	_00003_DS_
	BANKSEL	_GPIObits
	BCF	_GPIObits,0
_00003_DS_:
	BTFSS	STATUS,0
	GOTO	_00004_DS_
	BANKSEL	_GPIObits
	BSF	_GPIObits,0
_00004_DS_:
;	.line	41; "boton.c"	SHC = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,2
;	.line	42; "boton.c"	delay(5);
	MOVLW	0x05
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	43; "boton.c"	SHC = 0;
	BANKSEL	_GPIObits
	BCF	_GPIObits,2
;	.line	44; "boton.c"	delay(5);
	MOVLW	0x05
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	39; "boton.c"	for (unsigned char i = 7; i < 0xFF; i--) { // Usar < 0xFF para evitar problemas de comparación
	DECF	r0x1018,F
	GOTO	_00142_DS_
_00137_DS_:
;	.line	47; "boton.c"	STC = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,1
;	.line	48; "boton.c"	delay(5);
	MOVLW	0x05
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	49; "boton.c"	STC = 0;
	BANKSEL	_GPIObits
	BCF	_GPIObits,1
;	.line	50; "boton.c"	}
	RETURN	
; exit point of _shiftOutTwo

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;5 compiler assigned registers:
;   r0x1014
;   STK00
;   r0x1015
;   r0x1016
;   r0x1017
;; Starting pCode block
S_boton__delay	code
_delay:
; 2 exit points
;	.line	23; "boton.c"	void delay(unsigned int time) {
	MOVWF	r0x1014
	MOVF	STK00,W
	MOVWF	r0x1015
_00106_DS_:
;	.line	24; "boton.c"	while (time--) {
	MOVF	r0x1015,W
	MOVWF	r0x1016
	MOVF	r0x1014,W
	MOVWF	r0x1017
	MOVLW	0xff
	ADDWF	r0x1015,F
	BTFSS	STATUS,0
	DECF	r0x1014,F
	MOVF	r0x1017,W
	IORWF	r0x1016,W
	BTFSC	STATUS,2
	GOTO	_00112_DS_
;	.line	25; "boton.c"	for (unsigned int j = 0; j < 1275; j++);
	CLRF	r0x1016
	CLRF	r0x1017
;;unsigned compare: left < lit(0x4FB=1275), size=2
_00110_DS_:
	MOVLW	0x04
	SUBWF	r0x1017,W
	BTFSS	STATUS,2
	GOTO	_00131_DS_
	MOVLW	0xfb
	SUBWF	r0x1016,W
_00131_DS_:
	BTFSC	STATUS,0
	GOTO	_00106_DS_
;;genSkipc:3307: created from rifx:0x7ffece0fba20
	INCF	r0x1016,F
	BTFSC	STATUS,2
	INCF	r0x1017,F
	GOTO	_00110_DS_
_00112_DS_:
;	.line	27; "boton.c"	}
	RETURN	
; exit point of _delay


;	code size estimation:
;	  460+   59 =   519 instructions ( 1156 byte)

	end
