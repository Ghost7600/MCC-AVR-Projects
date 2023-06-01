#define F_CPU 16000000UL
#define GLOBAL_DEFINES

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/timer0.hpp"
#include "funsape/peripheral/timer1.hpp"
#include "funsape/device/sevenSegmentsDisplay.hpp"
#include "funsape/device/sevenSegmentsMuxDisplay.hpp"
#include "funsape/peripheral/pcint0.hpp"


// enum buttons {btn_m=1,btn_c,btn_d,btn_u};

uint32_t stopWatchGetTime(void);
vbool_t timerflag = 0;
vuint8_t num = 0;
vuint8_t nOvf = 0;
vuint16_t btnp = 0;
vuint8_t auxbtn = 0;

int main()
{
    uint16_t valor = 1802;
    uint8_t digitos[4];

    //Disable USART (Fix to solve bootloader issue)
    // UCSR0A = 0;
    // UCSR0B = 0;

    //Config INT0
    DDRB = 0x00;    // Setting portB as input for all pins
    PORTB = 0xFF;   // setting weak high state
    // setBit(PCICR, PCIE0); //Enables PCI0, happens when PCINT7..0 changes
    // PCMSK0 = 0b00001111; // enabling PCINT 0,1,2,3

    //Timer1 configuration
    Timer1 timer1;
    timer1.setMode(Timer1::Mode::NORMAL);
    timer1.setClockSource(Timer1::ClockSource::PRESCALER_8); //Timer Period should be arround 16ms
    timer1.activateOverflowInterrupt();
    setBit(DDRB, PB5);

    //Display Setup
    SevenSegmentsMuxDisplay sseg;
    sseg.setPorts(&PORTD, &PORTC, PC0, LogicLevel::LOW);
    sseg.init(SevenSegmentsMuxDisplay::Digits::DIGITS_4, SevenSegmentsDisplayType::COMMON_ANODE);

    //Button Interruptions Setup
    //Int0 buttonpress;
    // int0.init(Int0::SenseMode::FALLING_EDGE);
    // int0.activateInterrupt();
    Pcint0 buttonpress;
    buttonpress.init(Pcint0::Pin::PIN_PCINT0);
    buttonpress.enablePins(Pcint0::Pin::PIN_PCINT1);
    buttonpress.enablePins(Pcint0::Pin::PIN_PCINT2);
    buttonpress.enablePins(Pcint0::Pin::PIN_PCINT3);
    buttonpress.activateInterrupt();


    sei(); // Enable interrupt

    while(1) {
        valor = valor + btnp;
        btnp = 0;

        // Ciclic overflow
        if(valor > 9999) {
            valor = valor - 10000;
        }
        sseg.updateDigitValues(digitos);


        //On timer Overflow must go to next digit of display and every few refreshes must update display value
        if(timerflag) {
            sseg.showNextDigit();
            if(nOvf > 15) {
                //valor++;
                uint16_t aux = valor;
                for(uint8_t i = 0; i < 4; i++) {
                    digitos[i] = aux % 10;
                    aux /= 10;
                }
                nOvf = 0;
            }
            timerflag = 0;
        }

        // for(uint8_t t = 0; t < 8; t++) {
        //     sseg.showNextDigit();
        //     delayMs(16);
        // }
    }
    return 0;
}

void pcint0InterruptCallback()
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
    pcint0.clearInterruptRequest();
}


void timer1OverflowCallback(void)
{
    nOvf++;
    timerflag = 1;
    timer1.setCounterValue(0);
}
