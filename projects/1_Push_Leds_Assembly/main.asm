; ------------------------------------------------------------------------------
; Project:	LED Blink
; File:		ledBlink.asm
; Author:	Leandro Schwarz
; Created:	2017-08-07
; Modified:	2017-08-07
; Version:	1.0
; Notes:	Blinks a LED. Fcpu = 16 MHz.
; ------------------------------------------------------------------------------

; Include definition files
; ------------------------------------------------------------------------------
.include "include/m328Pdef.inc"

.equ BTNDDR = DDRB
.equ BTNPORT = PORTB
.equ BTNPIN = PINB
.equ BTNBIT = PB2


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

.def pusher = r16    ; Define register r16 as pusher
.def pushed = r17    ; Define register r17 as pushed
.def aux = r19
.def aux2 = r20



main:
    ;setting portd as output
    ldi aux ,0xFF    ;
    out DDRD, aux;
    ;setting pusher and pushed
    ldi pusher, 0b1  ; Initialize pusher with starting position
    ldi pushed, 0b10 ; Initialize pushed with 0b10

main_loop:

    ;outputing pusher or pushed
    ldi aux, 0
    mov aux ,pusher
    or aux, pushed
    out PORTD, aux
    ;finished outputing to leds

    ;calling delay
    call delay

    ;testing if pusher = pushed
    cp pusher, pushed
    breq pusher_eq_pushed;
    brne different;
    jmp main_loop


different:
    clc; clear carry flag
    rol pusher
    jmp main_loop

pusher_eq_pushed:
    ;check if they are at last ledBit
    ldi aux, 0b10000000
    cp pushed, aux
    brne not_last_led
    jmp main

not_last_led:
    ;move pushed one to the left and returns puser to beginning
    clc;
    rol pushed
    ldi pusher, 0b1
    jmp main_loop

delay:
    LDI     R20, 21 ; Initiate the Delay Register (250ms)
    LDI     R19, 74 ; 4000000 Cycles to Get 250 ms Delay
    LDI     R18, 190 ;)
delay_loop:
    DEC     R18 ; Decrements the Register
    BRNE    delay_loop ; If The Last Operation, isn't 0, go to delay_loop
    DEC     R19 ; Decrements the Register
    BRNE    delay_loop ; If The Last Operation, isn't 0, go to delay_loop
    DEC     R20 ; Decrements the Register
    BRNE    delay_loop ; If The Last Operation, isn't 0, go to delay_loop
    NOP ; Used to Add a Cycle to Delay
    NOP ;
    RET;

pcint0Handler:
    ; call debounceButton
    ; brne buttonReleased
    ; reti

resumeExecution:
    sei                         ; Enable interrupts
    rjmp main
; ------------------------------------------------------------------------------
; Interrupt handlers
; ------------------------------------------------------------------------------
int0Handler:
int1Handler:
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
    rjmp main;
