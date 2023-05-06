#define F_CPU 16000000UL


#include <avr/io.h>
#include <util/delay.h>
#include "sevenSegments.h"
#include "funsape/globalDefines.hpp"
#include "funsape/pinout.hpp"

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



    //setup
    UCSR0B = 0;     //USART Control Status register, probably for recording
    DDRD = 0xFF;    // Setting portD as output for all pins
    PORTD = 0x00;   // setting all bits of PORTD as 0
    clrbit(BTNDDR, BTNBIT);
    setbit(BTNPORT, BTNBIT);

    uint8_t pusher = 0b1;      //starting position lshifted by 1
    uint8_t pushed = 0b10;     //sarting position left shifted by 1
    uint8_t direction = 0; // 0 = up, 1 = down

    while(1) {


        PORTD = pusher | pushed;
        _delay_ms(500);

        //main loop
        if((pusher == pushed) && (pushed == 0b10000000)) {
            pusher = 0b1;      //starting position
            pushed = 0b10;
            // PORTD = pusher | pushed;
            // _delay_ms(500);
        }
        else{
            if(pushed == pusher) {
                pushed = pushed << 1;
                pusher = 0b1;
            }
            else {
                pusher = pusher << 1;
            }
        }

    }
    return 0;
};
