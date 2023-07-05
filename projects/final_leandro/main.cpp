
#define F_CPU 16000000UL

#include <util/delay.h>
#include <avr/interrupt.h>
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/usart0.hpp"
#include "zfm20.hpp"
#include "funsape/peripheral/timer0.hpp"
#include "funsape/peripheral/int0.hpp"
#include "kbitbang.hpp"
// #include <xc.h>
// #include <avr/interrupt.h>
// #include <util/delay.h>
// #include <stdio.h>
// #include "inc/LCD_interface.h"
// #include "inc/UART_interface.h"
// #include "inc/FP_interface.h"
// #include "inc/keys_interface.h"



int main()
{
    usart0.setBaudRate(Usart0::BaudRate::BAUD_RATE_57600);
    usart0.enableTransmitter();
    usart0.enableReceiver();
    usart0.init();
    usart0.stdio();
    //printf("USART OK \r\n");
    // UART_InterrptInit(57600);
    // lcdbegin();
    // keys_init();
    Zfm20 zfm20;
    uint8_t auxString[14];
    uint8_t testdata=0x01;

    //////////////////////// Configuração do USART de software para bitbang de debugacao/////////////////////
    //Timer Configuration
    DDRD = 0xFF;
    setBit(PORTD, PD2);
    Timer0 timer0;
    timer0.setMode(Timer0::Mode::CTC_OCRA);
    timer0.setClockSource(Timer0::ClockSource::PRESCALER_8);
    timer0.setCompareAValue(208);

    sei();
    delayMs(100);

    sendstring("Hello ");

    delayMs(1000);

    zfm20.init();
    auxString[0] = 0x18;
    auxString[1] = 0x04;
    auxString[2] = 0x13;
    auxString[3] = 0x23;
    auxString[4] = 0x56;
    auxString[5] = 0xFF;
    auxString[6] = 0x00;

    //zfm20.createPackage(Zfm20::Id::COMMAND, auxString, 7);
    //zfm20.receive_pkg(&usart0);
    // lcdclear();
    // lcdprint("Loading.. ");
    // _delay_ms(500);
    // sendcmd2fb(FB_connect);
    // _delay_ms(500);
    // unsigned char options_conter = 0;
    // lcdclear();

    while(1) {
        //zfm20.debug();
        //printf("loop restart");
        //zfm20.createPackage(Zfm20::Id::COMMAND, auxString, 7);
        //zfm20.debug();
        //zfm20.sendPackage(&usart0);
        //zfm20.receive_pkg(&usart0);
        zfm20.checkFingerPrint(&usart0);
        delayMs(3000);

    }
}
