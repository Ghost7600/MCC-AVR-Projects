
//--Definições--//
#define F_CPU 16000000UL
#define BUTTON_DDR DDRB
#define BUTTON_PORT PORTB
#define BUTTON_PIN PINB
#define BUTTON_BIT PB2

#define clrBit(auxReg, auxBit)              ((auxReg) &=(~(1 << (auxBit))))
#define setBit(auxReg, auxBit)              ((auxReg) |=(~(1 << (auxBit))))
#define clpBit(auxReg, auxBit)              ((auxReg) ^=(~(1 << (auxBit))))
#define isBitSet(auxReg, auxBit)            (((auxReg) >> (auxBit)) & 0x01)
#define isBitclr(auxReg, auxBit)            (!isBitSet(auxReg, auxBit))
#define waitUntilBitIsSet(auxReg, auxBit)   do{}while(isbitClr(auxReg, auxBit))
#define waitUntilBitIsClr(auxReg, auxBit)   do{}while(isBitSet(auxReg, auxBit))

//--Bibliotecas--//
#include <avr/io.h>
#include <util/delay.h>
#include "SEVENSEGMENTS.h"

int main(void)
{

    //--Variaveis--//
    uint8_t counter = 0;
    uint8_t direction = 0; // 0 = Up, 1 = DOWN

    UCSR0B = 0;   //Disable USART
    DDRD = 0XFF;  //PortD as outputs
    clrBit(BUTTON_DDR, BUTTON_BIT);
    setBit(BUTTON_PORT, BUTTON_BIT);

    while(1) {
        PORTD = hex2SevenSegments(counter, DISPLAY_COMMON_ANODE, 0);
        if(isBitclr(BUTTON_PIN, BUTTON_BIT)) {
            direction = !direction;
            _delay_ms(5);
            waitUntilBitIsClr(BUTTON_PIN, BUTTON_BIT);
        }
        counter = (direction) ?
                (counter == 0x00) ? 0x0F : (counter - 1) :
                (counter == 0x0F) ? 0x00 : (counter + 1);

        _delay_ms(250);
    }
    return 0;
}
