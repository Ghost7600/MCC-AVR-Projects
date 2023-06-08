//---Definições---//
#define F_CPU 16000000UL

//---INCLUDES---//
#include "funsape/globalDefines.hpp"

int main()
{
    //Variáveis locais
    uint8_t counter = 0;

    //Configurar os pinos do display
    setMask(DDRB, 0X0F);
    clrMask(PORTB, 0X0f);

    //Configurar o pino do botão
    clrBit(DDRD, PD2);
    setBit(PORTD, PD2);

    while(1) {

        clrMask(PORTB, 0X0F);
        setMask(PORTB, counter);

        counter = (counter < 15) ? (counter + 1) : 0;
        _delay_ms(300);

    }

}
