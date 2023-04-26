#define F_CPU 16000000UL


#include <avr/io.h>
#include <util/delay.h>
#include "sevenSegments.h"
#include "funsape/globalDefines.hpp"
#include "funsape/pinout.hpp"
#include "sevenSegments.h"

#define BTNDDR   DDRB
#define BTNPORT  PORTB
#define BTNPIN   PINB
#define BTNBIT   PB2

#define clrbit(auxreg,auxbit)   ((auxreg)&= (~(1<<(auxbit))))
#define setbit(auxreg,auxbit)   ((auxreg) |= (1 << (auxbit)))
#define cplbit(auxreg,auxbit)   ((auxreg) ^= (1 << (auxbit)))
#define isbitset(auxreg,auxbit) (((auxreg) >> (auxbit))& 0x1)
#define isbitclr(auxreg,auxbit) (~(((auxreg) >> (auxbit))& 0x1))
#define waitWhileClr(auxreg,auxbit)   do{}while(isbitclr(auxreg,auxbit))
#define waitWhileSet(auxreg,auxbit)   do{}while(isbitset(auxreg,auxbit))

int main()

{

    uint8_t counter = 0;
    uint8_t segments = 0;
    uint8_t direction = 0; // 0 = up, 1 = down

    //setup
    UCSR0B = 0;
    DDRD = 0xff;
    PORTD = 0xFF;
    clrbit(BTNDDR, BTNBIT);
    setbit(BTNPORT, BTNBIT);


    while(1) {
        PORTD = hex2SevenSegments(counter, DISPLAY_COMMON_ANODE, 0);
        // if(isbitclr(BTNPIN, BTNBIT)) {
        //     _delay_ms(5);
        //     direction = !direction;
        //     waitWhileClr(BTNPIN, BTNBIT);
        // }
        counter = counter >= 9 ? 0 : (counter + 1) ;
        _delay_ms(500);
    }




    return 0;
};
