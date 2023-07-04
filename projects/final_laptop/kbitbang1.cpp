#include "kbitbang1.hpp"


vbool_t timer1flag;

void bitbang(uint8_t tecla)
{
    timer1flag = true;
    timer1.clearCompareAInterruptRequest();
    timer1.setCounterValue(0);
    clrBit(PORTD, PD3);
    timer1.activateCompareAInterrupt();
    while(timer1flag) {}
    //delayUs(104);
    timer1.deactivateCompareAInterrupt();
    timer1flag = 1;
    for(uint8_t i = 0; i < 8; i++) {
        if(isBitSet(tecla, i)) {
            setBit(PORTD, PD3);
            timer1.activateCompareAInterrupt();
            while(timer1flag) {
                doNop();
            }
            timer1.deactivateCompareAInterrupt();
            timer1flag = 1;
        } else {
            clrBit(PORTD, PD3);
            timer1.activateCompareAInterrupt();
            while(timer1flag) {
                doNop();
            }
            timer1.deactivateCompareAInterrupt();
            timer1flag = 1;
        }
    }
    setBit(PORTD, PD3);
    timer1.activateCompareAInterrupt();
    while(timer1flag) {
    }
    timer1.deactivateCompareAInterrupt();
}

void sendstring(char string[64])
{
    int i = 0;
    while(string[i] != '\0') {
        bitbang(string[i]);
        i++;
    }
    bitbang('\n');
}

void timer1CompareACallback(void)
{
    timer1flag = 0;
    // cplBit(PORTD, PD3);
}
