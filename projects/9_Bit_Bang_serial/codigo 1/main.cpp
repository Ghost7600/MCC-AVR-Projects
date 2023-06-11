// Controller 1 - Keypad/Transmitter Code

//---DEFINIÇÔES---//
#define F_CPU 16000000UL

//---INCLUDES---//
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/int0.hpp"
#include "funsape/peripheral/timer0.hpp"
#include "funsape/device/keypad.hpp"

//global variables
vbool_t timer0flag;

void bitbang(uint8_t tecla)
{
    timer0flag = 1;
    timer0.setCounterValue(0);
    clrBit(PORTD, PD2);
    timer0.activateCompareAInterrupt();
    delayUs(104);
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
        } else {
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





//---MAIN---//
int main()
{
    timer0flag = 1;
    //Config Keypad
    Keypad keypad;
    keypad.setPorts(&DDRC, PC0, &DDRB, PB0);
    keypad.setKeyValues(Keypad::Type::KEYPAD_4X4,
            '7', '8', '9', 'A',
            '4', '5', '6', 'B',
            '1', '2', '3', 'C',
            'E', '0', 'F', 'D');
    keypad.init(10);
    uint8_t tecla;

    // Enabling interrupts
    sei();
    //Timer Configuration
    DDRD = 0xFF;
    setBit(PORTD, PD2);

    Timer0 timer0;
    timer0.setMode(Timer0::Mode::CTC_OCRA);
    timer0.setClockSource(Timer0::ClockSource::PRESCALER_8);
    timer0.setCompareAValue(208);
    while(1) {
        //if (keypad.readKeyPressed(&tecla)){
        keypad.readKeyPressed(&tecla);
        switch(tecla) {
        case 0xFF:
            break;

        default:
            bitbang(tecla);
            break;
        }
        //}
    }

    return 0;
}

void timer0CompareACallback(void)
{
    timer0flag = 0;
    // cplBit(PORTD, PD2);
}
