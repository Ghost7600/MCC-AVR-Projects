
#include <avr/io.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "funsape/globalDefines.hpp"
#include "funsape/pinout.hpp"
#include "funsape/device/hd44780.hpp"
#include "funsape/peripheral/int0.hpp"

#define F_CPU 16000000UL
#define LEDDIR  DDRC
#define LEDOUT  PORTC
#define LEDBIT  PC0

#define LCD_DATA_DIR    DDRB
#define LCD_DATA_OUT    PORTB
#define LCD_DATA_FIRST  PB0

#define LCD_CONTROL_DIR DDRB
#define LCD_CONTROL_OUT PORTB
#define LCD_CONTROL_E   PB4
#define LCD_CONTROL_RS  PB5

typedef union {
    struct {
        bool_t updateDisplay    : 1;
        bool_t readRTC          : 1;
        uint8_t unusedBits      : 6;
    };
    uint8_t allFlags;
} systemflags_t;

//global vars
volatile systemflags_t systemFlags;
uint8_t counter = 0;



int main()
{
    //local variables
    Hd44780 lcd;

    //clear system flags
    systemFlags.allFlags = 0;

    //Config LCD for test
    setBit(LEDDIR, LEDBIT);
    clrBit(LEDOUT, LEDBIT);

    //configure LCD
    lcd.controlPortSet(&LCD_CONTROL_DIR, LCD_CONTROL_E, LCD_CONTROL_RS);
    lcd.init(
            Hd44780::Size::LCD_16X2,
            Hd44780::Font::FONT_5X8,
            true,
            false);
    //show splash screen
    lcd.stdio();
    printf("Clock Example \n");
    printf("v1.0 build238 \n");
    delayMs(2000);
    lcd.clearScreen();
    systemFlags.updateDisplay = true;

    //config int0
    int0.init(Int0::SenseMode::FALLING_EDGE);
    int0.activateInterrupt();

    while(1) {
        //event machine
        if(systemFlags.updateDisplay == true) {
            //updates display
            printf("%u\n\n", counter);
            systemFlags.updateDisplay = false;
        }
        if(systemFlags.readRTC) {
            //read rtc
            counter++;
            systemFlags.updateDisplay = true;
            //clar rtc flag
            systemFlags.readRTC = false;
        }
    }
    return 0;
}
