#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/int0.hpp"
#include "funsape/peripheral/timer0.hpp"
#include "kbitbang.hpp"


vbool_t timer0flag;

void bitbang0(uint8_t tecla)
{
    timer0flag = true;
    timer0.clearCompareAInterruptRequest();
    timer0.setCounterValue(0);
    clrBit(PORTD, PD2);
    timer0.activateCompareAInterrupt();
    while(timer0flag){}
    //delayUs(104);
    timer0.deactivateCompareAInterrupt();
    timer0flag = 1;
    for(uint8_t i = 0; i < 8; i++) {
        if(isBitSet(tecla, i)) {
            setBit(PORTD, PD2);
            timer0.activateCompareAInterrupt();
            while(timer0flag) {
                doNop();
            }
            timer0.deactivateCompareAInterrupt();
            timer0flag = 1;
        }
        else {
            clrBit(PORTD, PD2);
            timer0.activateCompareAInterrupt();
            while(timer0flag) {
                doNop();
            }
            timer0.deactivateCompareAInterrupt();
            timer0flag = 1;
        }
    }
    setBit(PORTD, PD2);
    timer0.activateCompareAInterrupt();
    while(timer0flag) {
    }
    timer0.deactivateCompareAInterrupt();
}

void sendstring0 (char string[64]){
    int i =0;
    while (string[i] != '\0'){
        bitbang(string[i]);
        i++;
    }
    bitbang('\n');
}

void timer0CompareACallback(void)
{
    timer0flag = 0;
    // cplBit(PORTD, PD2);
}
