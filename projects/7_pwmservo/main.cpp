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

    //Config Keypad
    Keypad keypad;
    keypad.setPorts(&DDRD,PD0,&DDRD,PD4);
    keypad.setKeyValues(Keypad::Type::KEYPAD_4X4,
            '7', '8', '9', 'A',
            '4', '5', '6', 'B',
            '1', '2', '3', 'C',
            'E', '0', 'F', 'D');
    keypad.init(10);




    //Configure LCD
    lcd.controlPortSet(&DDRC, PC1, PC2, PC3);
    lcd.dataPortSet(&DDRB, PB2);
    lcd.init(Hd44780::Size::LCD_16X2, Hd44780::Font::FONT_5X8, true, true);
    lcd.stdio();

    printf("   Good Morning! :-)   \n\n");
    delayMs(500);
    lcd.clearScreen();
    setBit(DDRB,PB1);
    setBit(DDRB,PB2);
    Timer1 timer1;
    timer1.setOutputMode(Timer1::OutputMode::NON_INVERTING_MODE,
            Timer1::OutputMode::NORMAL);   // Fast-PWM, non inverting mode, normal
    timer1.init(Timer1::Mode::PWM_FAST_ICR, Timer1::ClockSource::PRESCALER_8);
    timer1.setInputCaptureValue(0xFFFF); //Define ICR, portanto período da onda.
    timer1.setCompareAValue(42000);           // Define a largura do pulso, Largura do Pulso(ms) = OCRA/2000, 21ms
    timer1.setCompareAValue(20000);           // Define a largura do pulso, Largura do Pulso(ms) = OCRA/2000, 21ms
    //posição do servo f(x)=({x-2000}/40000-0.5)*270 p/ range de -135 a +135  ou ainda pos = ({x-bottom}/(top-bottom)-0.5)*range
    //cada grau ~= 223 contas p 180 graus
    //para 270 graus cada 1 grau = 148 contas
    //para um angulo ang, o valor de OCRA1 deve ser ({ang+135}/135)/2*40000+2000
    int aux = 1999;
    timer1.setCompareAValue(aux);










    while(1) {
        OCR1A = 35333; // Set position to minimum degrees, 1ms pulse width
        // _delay_ms(500);
        // aux = aux + 148;
        // if (aux > 42000){
        //     aux = 1999;
        // }
    }

    return 0;
}
