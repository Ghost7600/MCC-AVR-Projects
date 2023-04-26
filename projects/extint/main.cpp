#define F_CPU 16000000UL

#include "funsape/globalDefines.hpp"

int main()
{
    //local variables
    uint8_t counter = 0;

    //configurando pinos do display
    setMask(DDRB, 0X0F);
    clrMask(PORTB, 0X0F);

    //configurando bit do botão
    clrBit(DDRD, PD2);
    setBit(PORTD, PD2);


    while(1) {
        clrMask(PORTB, 0X0F);
        setMask(PORTB, counter);
        counter = (counter < 15) ? (counter + 1) : 0;
        _delay_ms(500);
    }
    return 0;
};
