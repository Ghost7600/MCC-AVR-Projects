#define F_CPU 16000000UL
//#define GLOBAL_DEFINES

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
vbool_t short_long;
vbool_t timerflag = 0;
vbool_t timer1flag = 0;
vbool_t btnflag = 0;
vuint8_t num = 0;
vuint8_t menustate = 0;
vuint16_t nOvf0 = 0;
vuint16_t nOvf1 = 0;
vuint16_t btnp = 0;
vuint8_t auxbtn = 0;
vuint16_t valor = 0;

int main()
{
    uint16_t hora = 12;
    uint16_t minuto = 34;
    uint16_t segundo = 56;

    //Disable USART (Fix to solve bootloader issue)
    UCSR0A = 0;
    UCSR0B = 0;

    //Config INT0
    clrBit(DDRD, PD2); // Pin as input
    setBit(PORTD, PD2); //Pin as input with pull-up
    clrMaskOffset(EICRA, 0b11, ISC00); // Clear to configure the sense later
    // setMaskOffset(EICRA, 2, ISC00); // configure INT0 trigger sense at falling edge
    // setBit(EIMSK, INT0); // Enable INT0 service routine


    //Timer1 configuration
    // Timer1 timer1;
    // timer1.setMode(Timer1::Mode::NORMAL);
    // timer1.setClockSource(Timer1::ClockSource::PRESCALER_8); //Timer Period should be around 16ms
    // timer1.activateOverflowInterrupt();
    // setBit(DDRB, PB5);

    //Timer0 Config
    Timer0 timer0;
    timer0.setMode(Timer0::Mode::CTC_OCRA);
    timer0.setClockSource(Timer0::ClockSource::PRESCALER_1024);
    timer0.activateCompareAInterrupt();
    timer0.setCompareAValue(155);

    //Timer1 Config
    Timer1 timer1;
    timer1.setMode(Timer1::Mode::CTC_OCRA);
    timer1.setClockSource(Timer1::ClockSource::PRESCALER_1024);
    timer1.setCompareAValue(155);
    //Display Setup
    Hd44780 display;
    display.controlPortSet(&DDRB, PB4, PB5);
    display.dataPortSet(&DDRB, PB0);
    display.init(Hd44780::Size::LCD_16X2, Hd44780::Font::FONT_5X8, true, false);
    display.stdio();

    // Button Interruptions Setup
    // Int0 buttonpress;
    // int0.init(Int0::SenseMode::FALLING_EDGE);
    // int0.activateInterrupt();
    Pcint2 buttonpress;
    buttonpress.init(Pcint2::Pin::PIN_PCINT18);
    buttonpress.enablePins(Pcint2::Pin::PIN_PCINT18);
    buttonpress.activateInterrupt();

    // Splash screen
    printf(" %02d/%02d/20%02d %3s\n", hora, minuto, segundo, "SEG");
    printf("    %02d:%02d:%02d    \n", hora, minuto, segundo);
    sei(); // Enable interrupts

    while(1) {
        // valor = valor + btnp;
        // btnp = 0;

        // if(auxbtn == 1) {
        // display.clearScreen();
        // printf("%d\n",valor);
        // //delayMs(10);
        // auxbtn = 0;
        // }

        if(btnflag) { ///////MENU///////MENU//////////////MENU////////////MENU////////////// MENU///////////
            switch(menustate) {
            case 0:
                if(short_long == 1) {
                    timer0.deactivateCompareAInterrupt();
                    menustate = 1;
                    display.clearScreen();
                    printf("    hh:%02d:%02d    \n", minuto, segundo);
                    delayMs(300);
                }
                break;
            case 1:
                display.clearScreen();
                printf("    hh:%02d:%02d    \n", minuto, segundo);
                delayMs(300);
                if(short_long == 0) {
                    display.clearScreen();
                    hora++;
                    if(hora == 24) {
                        hora = 0;
                    };
                    printf("    %02d:%02d:%02d    \n", hora, minuto, segundo);
                    delayMs(300);
                }
                if(short_long == 1) {
                    menustate = 2;
                    display.clearScreen();
                    printf("    %02d:MM:%02d    \n", hora, segundo);
                    delayMs(125);
                }
                break;
            case 2:
                if(short_long == 0) {
                    display.clearScreen();
                    minuto++;
                    if(minuto == 60) {
                        minuto = 0;
                    };
                    printf("    %02d:%02d:%02d    \n", hora, minuto, segundo);
                    delayMs(100);
                }
                if(short_long == 1) {
                    display.clearScreen();
                    printf("    %02d:%02d:SS    \n", hora, minuto);
                    delayMs(125);
                    menustate = 3;
                }
                break;
            case 3:
                if(short_long == 0) {
                    display.clearScreen();
                    segundo++;
                    if(segundo == 60) {
                        segundo = 0;
                    };
                    printf("    %02d:%02d:%02d    \n", hora, minuto, segundo);
                    delayMs(300);
                }
                if(short_long == 1) {
                    display.clearScreen();
                    printf("Hora Gravada :-)");
                    menustate = 0;
                    delayMs(300);
                    display.clearScreen();
                    timer0.activateCompareAInterrupt();
                }
                break;
            default:
                break;
            }
            nOvf1 = 0;
            btnflag = 0;
        }

        //On timer Overflow must go to next digit of display and every few refreshes must update display value
        if(timerflag) {
            if(nOvf0 > 100) {
                nOvf0 = 0;
                segundo++;
                if(segundo == 60) {
                    segundo = 0;
                    minuto++;
                    if(minuto == 60) {
                        hora++;
                        minuto = 0;
                        if(hora == 24) {
                            hora = 0;
                        }
                    }
                }
                display.clearScreen();
                display.clearScreen();
                printf("    %02d:%02d:%02d    \n", hora, minuto, segundo);
            }
            timerflag = 0;
        }
    }
    return 0;
}

void pcint2InterruptCallback()
{
    if(isBitClr(PIND, PIND2)) {
        auxbtn = 1;
        nOvf1 = 0;
        timer1.activateCompareAInterrupt();
    } else {
        timer1.deactivateCompareAInterrupt();
        if(nOvf1 > 100) {
            short_long = 1;
        } else {
            short_long = 0;
        }

        btnflag = 1;
    }
    pcint2.clearInterruptRequest();
}

void timer1CompareACallback(void)
{
    nOvf1++;
    timer1flag = 1;
}
void timer0CompareACallback(void)
{
    nOvf0++;
    timerflag = 1;
}
