#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/timer0.hpp"
#include "funsape/peripheral/timer1.hpp"

void stopWatchStart(void);
void stopWatchStop(void);
uint32_t stopWatchGetTime(void);
vuint8_t num = 0;
vuint8_t nOvf = 0;

int main()
{
    uint16_t counter = 1820;

    //Disable USART (Fix to solve bootloader issue)
    UCSR0A = 0;
    UCSR0B = 0;

    //Config INT0
    DDRB = 0x00;    // Setting portB as input for all pins
    PORTB = 0xFF;   // setting weak high state
    setBit(PCICR, PCIE0) //Enables PCI0, happens when PCINT7..0 changes
    PCMSK0 = 0b00001111 // enabling PCINT 0,1,2,3

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
        // lcd.clearScreen();
        // printf("qqr coisa\n");
        // printf("Num=%d\n",num++)
        // cplBit(PORTB, PB5);
        // delayMs(250);
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
