; ------------------------------------------------------------------------------
; Project:	LED Blin
; File:		ledBlink.asm
; Author:	Fabio Schmitt Avelino
; Created:	2023-03-07
; Modified:	2023-03-07
; Version:	1.0
; Notes:	Blinks a LED. Fcpu = 16 MHz.
; ------------------------------------------------------------------------------
; Include definition files
; ------------------------------------------------------------------------------
.include "include/m168PAdef.inc"
; ------------------------------------------------------------------------------
; Register definitions
; ------------------------------------------------------------------------------
.cseg
.org 0x000
.def temp = r16 ; define temporary register
.def led = r17  ; define register pin LED
; ------------------------------------------------------------------------------
; Main code
; ------------------------------------------------------------------------------
start:
ldi temp, 0xff       ; set bit DDRD to 1
out DDRD, temp

animationstart:
ldi led, 0x01        ; ldi led, 0b0000 0001

animationloop:
out PORTD, led
rcall delay
lsl led              ; deslocamento dos registradores
brcs animationstart
rjmp animationloop      ; jump animationloop

delay:
ldi r18, 21            ; set a delay value (adjust as needed)
ldi r19, 75
ldi r20, 189

delay_loop:
dec r20                    ; decrement the delay value
brne delay_loop            ; loop until the delay value is zero
dec r19                    ; decrement the delay value
brne delay_loop            ; loop until the delay value is zero
dec r18
brne delay_loop
ret                        ; return from the subroutine
