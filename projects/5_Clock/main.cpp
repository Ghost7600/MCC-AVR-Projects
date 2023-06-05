#define F_CPU 16000000UL
#define GLOBAL_DEFINES

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/timer0.hpp"
#include "funsape/peripheral/timer1.hpp"
#include "funsape/device/hd44780.hpp"
#include "funsape/peripheral/pcint2.hpp"


// enum buttons {btn_m=1,btn_c,btn_d,btn_u};

uint32_t stopWatchGetTime(void);
vbool_t timerflag = 0;
vuint8_t num = 0;
vuint8_t nOvf = 0;
vuint16_t btnp = 0;
vuint8_t auxbtn = 0;
vuint16_t valor = 0;

int main()
{
    uint16_t hora = 10;
    uint16_t minuto = 30;
    uint16_t segundo = 37;
    uint8_t digitos[4];

    //Disable USART (Fix to solve bootloader issue)
    UCSR0A = 0;
    UCSR0B = 0;

    //Config INT0
    clrBit(DDRD, PD2); // Pin as input
    setBit(PORTD, PD2); //Pin as input with pull-up
    clrMaskOffset(EICRA, 0b11, ISC00); // Clear to configure the sense later
    setMaskOffset(EICRA, 2, ISC00); // configure INT0 trigger sense at falling edge
    setBit(EIMSK, INT0); // Enable INT0 service routine


    //Timer1 configuration
    Timer1 timer1;
    timer1.setMode(Timer1::Mode::NORMAL);
    timer1.setClockSource(Timer1::ClockSource::PRESCALER_8); //Timer Period should be arround 16ms
    timer1.activateOverflowInterrupt();
    // setBit(DDRB, PB5);

    //Display Setup
    Hd44780 display;
    display.controlPortSet(&DDRB, PB4, PB5);
    display.dataPortSet(&DDRB, PB0);
    display.init(Hd44780::Size::LCD_16X2, Hd44780::Font::FONT_5X8, true, false);

    //Button Interruptions Setup
    //Int0 buttonpress;
    // int0.init(Int0::SenseMode::FALLING_EDGE);
    // int0.activateInterrupt();
    // Pcint2 buttonpress;
    // buttonpress.init(Pcint2::Pin::PIN_PCINT18);
    // buttonpress.enablePins(Pcint2::Pin::PIN_PCINT18);
    // buttonpress.activateInterrupt();

    // Splash screen
    printf(" %02d/%02d/20%02d %3s\n", hora, minuto, segundo, "SEG");
    printf("    %02d:%02d:%02d    \n", hora, minuto, segundo);
    delayMs(2000);
    display.clearScreen();
    printf("Kenner");
    delayMs(2000);
    sei(); // Enable interrupt

    while(1) {
        valor = valor + btnp;
        btnp = 0;

        // Ciclic overflow
        if(valor > 9999) {
            valor = valor - 10000;
        }
        printf("digitos");


        // //On timer Overflow must go to next digit of display and every few refreshes must update display value
        // if(timerflag) {
        //     sseg.showNextDigit();
        //     if(nOvf > 15) {
        //         //valor++;
        //         uint16_t aux = valor;
        //         for(uint8_t i = 0; i < 4; i++) {
        //             digitos[i] = aux % 10;
        //             aux /= 10;
        //         }
        //         nOvf = 0;
        //     }
        //     timerflag = 0;
        // }

        // for(uint8_t t = 0; t < 8; t++) {
        //     sseg.showNextDigit();
        //     delayMs(16);
        // }
    }
    return 0;
}

void pcintInterruptCallback()
{


    switch(auxbtn) {
    case 0b1:
        btnp = 1000;
        break;

    case 0b10:
        btnp = 100;
        break;

    case 0b100:
        btnp = 10;
        break;

    case 0b1000:
        btnp = 1;
        break;
    }
    auxbtn = PINB;
    auxbtn = ~auxbtn;
    pcint2.clearInterruptRequest();
}


void timer1OverflowCallback(void)
{
    nOvf++;
    timerflag = 1;
    timer1.setCounterValue(0);
}
