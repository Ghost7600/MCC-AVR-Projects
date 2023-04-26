; ------------------------------------------------------------------------------
; Project:	LED Blink
; File:		ledBlink.asm
; Author:	Leandro Schwarz
; Created:	2017-08-07
; Modified:	2017-08-07
; Version:	1.0
; Notes:	Blinks a LED. Fcpu = 16 MHz.
; ------------------------------------------------------------------------------

; ------------------------------------------------------------------------------
; Include definition files
; ------------------------------------------------------------------------------
.include "include/m328Pdef.inc"
; ------------------------------------------------------------------------------
; Register definitions
; ------------------------------------------------------------------------------
.def	leds			= R16
.equ esquerda, 0x01
; ------------------------------------------------------------------------------
; Interruption handlers
; ------------------------------------------------------------------------------
.org 0x0000
	JMP resetHandler
.org INT0addr
	JMP int0Handler
.org INT1addr
	JMP int1Handler
.org PCI0addr
	JMP pcint0Handler
.org PCI1addr
	JMP pcint1Handler
.org PCI2addr
	JMP pcint2Handler
.org WDTaddr
	JMP wdtHandler
.org OC2Aaddr
	JMP timer2CompAHandler
.org OC2Baddr
	JMP timer2CompBHandler
.org OVF2addr
	JMP timer2OvfHandler
.org ICP1addr
	JMP timer1CaptureHandler
.org OC1Aaddr
	JMP timer1CompAHandler
.org OC1Baddr
	JMP timer1CompBHandler
.org OVF1addr
	JMP timer1OvfHandler
.org OC0Aaddr
	JMP timer0CompAHandler
.org OC0Baddr
	JMP timer0CompBHandler
.org OVF0addr
	JMP timer0OvfHandler
.org SPIaddr
	JMP spiHandler
.org URXCaddr
	JMP usartRxHandler
.org UDREaddr
	JMP usartDataEmptyHandler
.org UTXCaddr
	JMP usartTxHandler
.org ADCCaddr
	JMP adcHandler
.org ERDYaddr
	JMP eepromHandler
.org ACIaddr
	JMP analogCompHandler
.org TWIaddr
	JMP twiHandler
.org SPMRaddr
	JMP flashHandler
.org INT_VECTORS_SIZE

; ------------------------------------------------------------------------------
; Include other assembly files
; ------------------------------------------------------------------------------

; ------------------------------------------------------------------------------
; Constants stored in Flash memory
; Note: Variables must be multiples of 2, since memory is organized in 16 bits
; ------------------------------------------------------------------------------
;.def leds = R16
; ------------------------------------------------------------------------------
; Main code
; ------------------------------------------------------------------------------
main:
;setando portas
LDI R17,0XFF    ;carrega registrador auxiliar
OUT DDRD, R17   ;port direction, seta como saida
LDI leds, 0b1     ;carrega 1 no primeiro led
OUT PORTD, leds
ldi r15, esquerda
and r16, r15
out portd, leds

;corre led pra esquerda
;inicia led
;ldi leds,1



mainLoop:
    CALL delay500
    ROL leds
    OUT PORTD, leds
	JMP		mainLoop


; ------------------------------------------------------------------------------
; Interrupt handlers
; ------------------------------------------------------------------------------
int0Handler:
int1Handler:
pcint0Handler:
pcint1Handler:
pcint2Handler:
wdtHandler:
timer2CompAHandler:
timer2CompBHandler:
timer2OvfHandler:
timer1CaptureHandler:
timer1CompAHandler:
timer1CompBHandler:
timer1OvfHandler:
timer0CompAHandler:
timer0CompBHandler:
timer0OvfHandler:
spiHandler:
usartRxHandler:
usartDataEmptyHandler:
usartTxHandler:
adcHandler:
eepromHandler:
analogCompHandler:
twiHandler:
flashHandler:
resetHandler:
	LDI		R19, LOW(RAMEND)
	OUT		SPL, R19
	LDI		R19, HIGH(RAMEND)
	OUT		SPH,R19
	JMP		main
delay500:
; Assembly code auto-generated
; by utility from Bret Mulvey
; Delay 8 000 000 cycles
; 500ms at 16 MHz

    ldi  r18, 41
    ldi  r19, 150
    ldi  r20, 128
L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L1
ret
