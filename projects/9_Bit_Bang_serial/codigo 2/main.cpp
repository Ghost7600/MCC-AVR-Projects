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
#include "funsape/device/hd44780.hpp"

//global variables
vbool_t timer0flag;
vbool_t fint0 ;
vuint8_t tecla;
vuint8_t cursor;


void lerbits(void){
    timer0.setCounterValue(0);
    timer0.activateCompareAInterrupt(); //failsafe, verificar se da pra remover
    for (uint8_t i = 0; i < 8; i++)     //loop de ler bits e carregar no registrador
    {
        while (timer0flag){
        } //aguarda hora de ler
        timer0flag = 1;
        //cplBit(PORTC, PC0);
        // check bit
        if (isBitSet(PIND,PD2)){
             //LOAD BIT IN TECLA'S RESPECTIVE POSITION
             setBit(tecla,i);
             cplBit(PORTC, PC0);
        }
        else{
            clrBit(tecla,i);
            //cplBit(PORTC, PC0);
        }
    }
    timer0.deactivateCompareAInterrupt();
}


void lertecla (void){
    int0.deactivateInterrupt();
    // aguardar 52ms p/ meio do 1o bit
    delayUs(52);
    timer0.setCompareAValue(104);
    timer0.activateCompareAInterrupt();
    while (1)
    {
        if (timer0flag == 0){
            break;
        }
    }
    setBit(PORTC,PC0);
    timer0flag =1;
    //prepara para ler proximos bit
    timer0.setCompareAValue(208);
    lerbits();  //função de ler bits
    timer0.deactivateCompareAInterrupt();
    int0.activateInterrupt();
}


//---MAIN---//
int main()
{
    setBit(DDRC,PC0);
    fint0 =1;
    timer0flag = 1;
    //Config Keypad
    Keypad keypad;
    keypad.setKeyValues(Keypad::Type::KEYPAD_4X4,
            '7', '8', '9', 'A',
            '4', '5', '6', 'B',
            '1', '2', '3', 'C',
            'E', '0', 'F', 'D');


    //Timer Configuration
    DDRD = 0xFF;
    clrBit(DDRD,PD2);

    Int0 int0;
    int0.init(Int0::SenseMode::FALLING_EDGE);
    int0.activateInterrupt();

    Timer0 timer0;
    timer0.setMode(Timer0::Mode::CTC_OCRA);
    timer0.setClockSource(Timer0::ClockSource::PRESCALER_8);

    // Enabling interrupts
    sei();

    Hd44780 lcd;
    lcd.controlPortSet(&DDRC, PC1, PC2, PC3);
    lcd.dataPortSet(&DDRB, PB2);
    lcd.init(Hd44780::Size::LCD_16X2, Hd44780::Font::FONT_5X8, true, true);
    lcd.stdio();
    lcd.clearScreen();
    printf("Ola");
    delayMs(300);
    lcd.clearScreen();
    cursor =0;
    while(1) {
        if (fint0 ==0){
            fint0 =1;
            lertecla();
            if (cursor>16){
                lcd.clearScreen();
                cursor =0;
            }
            cursor++;
            if (tecla != 'E'){
                printf("%c", tecla);
            }
            else {
                lcd.clearScreen();
            }
        }
    }
    return 0;
}


void timer0CompareACallback(void){
    timer0flag = 0;
}

void int0InterruptCallback (void){
    fint0 =0;
}

//inicializar no trasnmitter as flags na main
