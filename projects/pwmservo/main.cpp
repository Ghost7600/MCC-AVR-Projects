
#include <avr/io.h>
#include "funsape/peripheral/timer1.hpp"
#include "funsape/globalDefines.hpp"
int main()
{
    //Config used pins as inputs in pull-up
    clrBit(DDRD, PD0); // Pin as input
    setBit(PORTD, PD0); //Pin as input with pull-up

    clrBit(DDRD, PD1); // Pin as input
    setBit(PORTD, PD1); //Pin as input with pull-up

    //interruption configurations
    clrMaskOffset(EICRA, 0b11, ISC00); // Clear to configure the sense later
    setMaskOffset(EICRA, 2, ISC00); // configure INT0 trigger sense at falling edge
    setBit(EIMSK, INT0); // Enable INT0 service routine

    clrMaskOffset(EICRA, 0b11, ISC10); // Clear to configure the sense later
    setMaskOffset(EICRA, 2, ISC10); // configure INT1 trigger sense at falling edge
    setBit(EIMSK, INT1); // Enable INT1 service routine

    //Timer1 configuration
    timer1.setMode(Timer1::Mode::PWM_FAST_8_BITS);
    timer1.setClockSource(Timer1::ClockSource::PRESCALER_8;
            timer1.activateOverflowInterrupt();
            setBit(TCCR1A, COM1A0); //
            clrBit(TCCR1A, COM1A1);

            //timer1 configuration
            // timer1.init();
            // timer1.setCompareAValue();
            // timer1.activateCompareAInterrupt();

    while(1) {

}

return 0;
}
