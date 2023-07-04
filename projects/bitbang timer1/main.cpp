
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/int1.hpp"
#include "funsape/peripheral/timer1.hpp"
#include "funsape/device/keypad.hpp"
#include "funsape/peripheral/usart0.hpp"
#include "kbitbang.hpp"

int main()
{
//////////////////////// Configuração do USART de software para bitbang de debugacao/////////////////////
    //Timer Configuration
    DDRD = 0xFF;
    setBit(PORTD, PD3);
    Timer1 timer1;
    timer1.setMode(Timer1::Mode::CTC_OCRA);
    timer1.setClockSource(Timer1::ClockSource::PRESCALER_8);
    timer1.setCompareAValue(208);

    sei();

    while (1)
    {
        sendstring("timer 1 bitbang funcionando!");
        // delayMs(40);
        sendstring("Uba yeah");
    }
    return 0;
}
