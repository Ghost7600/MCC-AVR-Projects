//Atividade Supermaquina utilizando registradores de deslocamento 74HC595 em C e C++

#define F_CPU 16000000UL


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "sevenSegments.h"
#include "funsape/globalDefines.hpp"
#include "funsape/pinout.hpp"

#define DATADDR   DDRD
#define DATAPORT  PORTD
#define DATAPIN   PIND
#define DATABIT   PD0

#define DATACLK   PD1
#define LATCH   PD2



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

    uint8_t counter = 0;
    while(1) {
        for(counter = 0; counter <= 15; counter++) { //laço para bits crescendo
            if(counter == 0) {
                setBit(DATAPORT, DATABIT); //set data bit for first cycle
            } else {
                clrBit(DATAPORT, DATABIT); //ensures data bit clear for remaining cycles
            }
            setBit(DATAPORT, DATACLK);  //pulses clock for pushing bit forward
            delayUs(10);
            clrBit(DATAPORT, DATACLK);
            delayUs(10);
            setBit(DATAPORT, LATCH);    //pulses latch for outputting data
            delayUs(10);
            clrBit(DATAPORT, LATCH);
            delayMs(250);               //frame delay
        }

        for(counter = 14; counter > 0; counter--) { //laço para bits decrescendo
            setBit(DATAPORT, DATABIT); //carrega primeiro bit
            setBit(DATAPORT, DATACLK);
            delayUs(10);
            clrBit(DATAPORT, DATACLK);
            clrBit(DATAPORT, DATABIT);
            for(int c2 = 0; c2 < counter; c2++) {   //desloca até a posição desejada, no caso 1 anterior ao contador
                setBit(DATAPORT, DATACLK);
                delayUs(10);
                clrBit(DATAPORT, DATACLK);
                delayUs(10);
            }
            setBit(DATAPORT, LATCH);    //outputs bit
            delayUs(10);
            clrBit(DATAPORT, LATCH);
            delayMs(250);
            if(counter <= 7) {      //quando lidamos com bits no ultimo quarto da animação
                for(int c2 = 0; c2 < 16; c2++) { //laço para zerar dados
                    setBit(DATAPORT, DATACLK);
                    delayUs(10);
                    clrBit(DATAPORT, DATACLK);
                }
            }
        }
    }
    return 0;
}
