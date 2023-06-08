; ------------------------------------------------------------------------------
; Project:	LED Blin
; File:		ledBlink.asm
; Author:	Fabio Schmitt Avelino
; Created:	2023-03-14
; Modified:	2023-03-14
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
main:
sbi DDRB, PB5

mainloop:
sbi PORTB, PB5
rcall delay
cbi PORTB, PB5
rcall delay
jmp mainloop

blinkled:


delay:
ldi r18, 41            ; set a delay value (adjust as needed)
ldi r19, 150
ldi r20, 125

delay_loop:
dec r20                    ; decrement the delay value
brne delay_loop            ; loop until the delay value is zero
dec r19                    ; decrement the delay value
brne delay_loop            ; loop until the delay value is zero
dec r18
brne delay_loop
rjmp PC + 1
ret                        ; return from the subroutine
