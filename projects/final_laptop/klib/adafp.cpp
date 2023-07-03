#include "adafp.hpp"

void usartsendbyte(uint8_t byte){
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = byte;
}

void usartsendstring (char string[64]){

    int i =0;
    while (string[i] != '\0'){
        while (!(UCSR0A & (1<<UDRE0)));
        UDR0 = string[i];
        i++;
    }
}

void signalbegin(){
    //mandando bautou????
    usartsendbyte(0xEF);
    usartsendbyte(0x01);
    //mandando endereço
    usartsendbyte(0xFF);
    usartsendbyte(0xFF);
    usartsendbyte(0xFF);
    usartsendbyte(0xFF);
}
