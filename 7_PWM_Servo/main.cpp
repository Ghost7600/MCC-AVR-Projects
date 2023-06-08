//---DEFINIÇÔES---//
#define F_CPU 16000000UL

//---INCLUDES---//
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/timer1.hpp"
#include "funsape/device/hd44780.hpp"
#include "funsape/device/keypad.hpp"


//---MAIN---//
int main()
{
    Hd44780 lcd;
    Keypad keypad;


    //Configure LCD
    lcd.controlPortSet(&DDRC, PC1, PC2, PC3);
    lcd.dataPortSet(&DDRB, PB2);
    lcd.init(Hd44780::Size::LCD_16X2, Hd44780::Font::FONT_5X8, true, true);
    lcd.stdio();

    printf("   KEYPAD ADC   \n\n");
    delayMs(500);
    lcd.clearScreen();

    Timer1 timer1;
    timer1.setOutputMode(Timer1::OutputMode::NON_INVERTING_MODE,
            Timer1::OutputMode::NORMAL);   // Fast-PWM, non inverting mode, normal
    timer1.init(Timer1::Mode::PWM_FAST_ICR, Timer1::ClockSource::PRESCALER_8);                  // prescaler = 8
    timer1.setInputCaptureValue(39999);                                                         // Value ICR=39999
    timer1.setCompareAValue(1999);           // Value OCR1A=1999
    lcd.clearScreen();
    printf("finished timer cofiguration");
    delayMs(500);
    while(1) {
        lcd.clearScreen();
        printf("entered while loop");
        OCR1A = 1999; // Set position to 0 degrees
        _delay_ms(500);
        lcd.clearScreen();
        OCR1A = 2999; // Set position to 90 degrees
        _delay_ms(500);
        OCR1A = 3999; // Set position to 180 degrees
        _delay_ms(500);
        OCR1A = 2999; // Set position to 90 degrees

    }

    return 0;
}
