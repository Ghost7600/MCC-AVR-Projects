// Programa para ler o valor dos botoes do ADC
// Apenas para teste.
//      1) Configurar ADC para modo "free running" (disparo automático
//          contínuo).
//      2) Ler ADC a cada 100ms usando atraso como gerador de base de tempo.
//      3) Mostrar valor do ADC no display.

#define F_CPU 16000000UL

#include "funsape/globalDefines.hpp"
#include "funsape/device/hd44780.hpp"
#include "funsape/peripheral/adc.hpp"

uint8_t keypadAdcDecoder(uint16_t adcValue_p);

int main()
{
    // Local variables
    Hd44780 lcd;
    Adc adc;

    DDRD = 0xFF;

    // Configure LCD
    lcd.controlPortSet(&DDRB, PB4, PB5);
    lcd.dataPortSet(&DDRB, PB0);
    lcd.init(Hd44780::Size::LCD_16X2, Hd44780::Font::FONT_5X8, true, false);
    lcd.stdio();
    printf("   KEYPAD ADC   \n\n");
    // delayMs(2000);
    lcd.clearScreen();

    // Configure ADC
    adc.init(Adc::Mode::AUTO_CONTINUOUS, Adc::Reference::POWER_SUPPLY, Adc::Prescaler::PRESCALER_128);
    adc.setChannel(Adc::Channel::CHANNEL_3);
    adc.enable();
    adc.startConversion();
    adc.activateInterrupt();

    sei();                                      // Enable global interrupts

    while(1) {
        delayMs(10);
        lcd.cursorHome();
        printf("ADC = %X\n\n", keypadAdcDecoder(ADC));
    }

    return 0;
}

// #include <avr/interrupt.h>

// ISR(ADC_vect) {

// }

uint8_t keypadAdcDecoder(uint16_t adcValue_p)
{
    uint8_t key = 0xFF;

    switch(adcValue_p) {

    case 552 ... 792:   key = 0x07;     break;
    case 533 ... 551:   key = 0x08;     break;
    case 513 ... 532:   key = 0x09;     break;
    case 492 ... 512:   key = 0x0A;     break;
    case 470 ... 491:   key = 0x04;     break;
    case 444 ... 469:   key = 0x05;     break;
    case 415 ... 443:   key = 0x06;     break;
    case 386 ... 414:   key = 0x0B;     break;
    case 354 ... 385:   key = 0x01;     break;
    case 315 ... 353:   key = 0x02;     break;
    case 272 ... 314:   key = 0x03;     break;
    case 227 ... 271:   key = 0x0C;     break;
    case 176 ... 226:   key = 0x0E;     break;
    case 114 ... 175:   key = 0x00;     break;
    case 41 ... 113:    key = 0x0F;     break;
    case 0 ... 40:      key = 0x0D;     break;
    default:            key = 0xFF;     break;
    }

    return key;
}
