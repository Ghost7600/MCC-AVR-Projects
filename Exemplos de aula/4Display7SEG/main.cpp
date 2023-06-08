//---Definições---//
#define F_CPU 16000000UL

//---INCLUDES---//
#include "funsape/globalDefines.hpp"
#include "funsape/util/sevenSegmentsDisplay.hpp"
#include <avr/interrupt.h>

int main()
{
    //Variáveis locais
    uint16_t counter = 0;

    //Configurar os pinos do diplay
    setMask(DDRD, 0X0F);
    clrMask(PORTD, 0X0f);

    //Configurar o pino do botão
    clrBit(DDRB, PB0);
    setBit(PORTB, PB0);

    while(1) {

        clrMask(PORTD, 0X0F);
        setMask(PORTD, counter);

        counter = (counter == 9) ? 0 : (counter + 1);
        _delay_ms(300);

    }

}

void display_digit(uint8_t digit , uint8_t value){
    // seleciona o digito do display
    switch(digit){
        case 0x00:          auxsegments = 0b00111111;  break;
        case 0x01:          auxsegments = 0b00000110;  break;
        case 0x02:          auxsegments = 0b01011011;  break;
        case 0x03:          auxsegments = 0b01001111;  break;
        case 0x04:          auxsegments = 0b01100110;  break;
        case 0x05:          auxsegments = 0b01101101;  break;
        case 0x06:          auxsegments = 0b01111101;  break;
        case 0x07:          auxsegments = 0b00000111;  break;
        case 0x08:          auxsegments = 0b01111111;  break;
        case 0x09:          auxsegments = 0b01101111;  break;
        case DISPLAY_DASH:  auxsegments = 0b01000000;  break;  // Dash
        case DISPLAY_U:     auxsegments = 0b00111110;  break;  // U
        case DISPLAY_OFF:   auxsegments = 0b00000000;  break;  // OFF
    }
    
    
}
