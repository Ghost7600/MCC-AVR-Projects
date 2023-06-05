#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "funsape/globalDefines.hpp"
#include "funsape/device/hd44780.hpp"
#include "funsape/peripheral/timer0.hpp"
#include "funsape/peripheral/timer1.hpp"

void stopWatchStart(void);
void stopWatchStop(void);
uint32_t stopWatchGetTime(void);
Hd44780 lcd;
vuint8_t num = 0;
vuint8_t nOvf = 0;

int main()
{
    uint16_t dia = 29;
    uint16_t mes = 4;
    uint16_t ano = 23;
    uint16_t hora = 10;
    uint16_t minuto = 30;
    uint16_t segundo = 37;

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
    timer1.setMode(Timer1::Mode::NORMAL);
    timer1.setClockSource(Timer1::ClockSource::DISABLED);
    timer1.activateOverflowInterrupt();

    //Config LCD
    lcd.controlPortSet(&DDRB, PB4, PB5);
    lcd.dataPortSet(&DDRB, PB0);
    lcd.init(Hd44780::Size::LCD_16X2, Hd44780::Font::FONT_5X8, true, false);
    lcd.stdio();

    // Splash screen
    printf(" %02d/%02d/20%02d %3s\n", dia, mes, ano, "SEG");
    printf("    %02d:%02d:%02d    \n", hora, minuto, segundo);
    delayMs(2000);

    lcd.clearScreen();
    printf("Kenner");

    setBit(DDRB, PB5);

    sei(); // Enable interrupt

    while(1) {
        stopWatchStart();
        delayMs(1000);
        lcd.clearScreen();
        printf("qqr coisa\n");
        //printf("Num=%d\n",num)
        cplBit(PORTB, PB5);
        delayMs(250);
        stopWatchStop();

        lcd.clearScreen();
        printf("time=%lu\n %u %u", stopWatchGetTime(), nOvf, TCNT1);
        delayMs(1000);
    }

    return 0;
}


ISR(INT0_vect)
{
    //stopWatchStart();
    lcd.clearScreen();
    printf("Outra coisa\n");
    printf("num=%d\n", num++);
    //stopWatchStop();

}


void timer1OverflowCallback(void)
{
    nOvf++;
    timer1.setCounterValue(0);
}

void stopWatchStart(void)
{
    //clear all vars
    nOvf = 0;
    timer1.setCounterValue(0);

    //timer1 run
    timer1.setClockSource(Timer1::ClockSource::PRESCALER_8);
}
void stopWatchStop(void)
{
    //timer1 stop
    timer1.setClockSource(Timer1::ClockSource::DISABLED);
}

uint32_t stopWatchGetTime(void)
{
    uint32_t totalTime = 0;
    totalTime = (uint32_t)nOvf * (0x0FFFFUL + 1) + (uint32_t)timer1.getCounterValue();

    return (totalTime / 2);
}
