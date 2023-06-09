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

//globals
uint32_t aux = 2999;
int16_t angulo;

int16_t typing(int16_t valor, uint8_t digitado, uint8_t subestado)
{
    switch(subestado) {
    case 1:
        valor = digitado;
        break;
    case 2:
        if(valor < 0) {
            valor = valor * 10 - digitado;
            break;
        } else {
            valor = valor * 10 + digitado;
            break;
        }

    case 3:
        if(valor < 0) {
            valor = valor * 10 - digitado;
            break;
        } else {
            valor = valor * 10 + digitado;
            break;
        }
    default:
        if(valor < 0) {
            valor = valor * 10 - digitado;
            break;
        } else {
            valor = valor * 10 + digitado;
            break;
        }
    }
    return valor;
}

void setangle(uint16_t angulo)
{
    aux = ((100 * angulo + 13500) / 135) * 10 + 1999;
    timer1.setCompareAValue(aux);
}

int16_t getangle(uint32_t aux)
{
    int32_t aux2;
    int16_t angulo;
    aux2 = 10 * aux - 29999;
    angulo = 10 * aux2 / 741;
    if(angulo > 0) {
        angulo++;
    }
    if(angulo == 1) {
        angulo = 0;
    }
    return angulo;
}

//---MAIN---//
int main()
{
    Hd44780 lcd;

    //Config Keypad
    Keypad keypad;
    keypad.setPorts(&DDRD, PD4, &DDRD, PD0);
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
    setBit(DDRB, PB1);
    setBit(DDRB, PB2);
    Timer1 timer1;
    timer1.setOutputMode(Timer1::OutputMode::NON_INVERTING_MODE,
            Timer1::OutputMode::NORMAL);   // Fast-PWM, non inverting mode, normal
    timer1.init(Timer1::Mode::PWM_FAST_ICR, Timer1::ClockSource::PRESCALER_8);
    timer1.setInputCaptureValue(0xFFFF); //Define ICR, portanto período da onda.
    timer1.setCompareAValue(42000);           // Define a largura do pulso, Largura do Pulso(ms) = OCRA/2000, 21ms
    timer1.setCompareAValue(20000);           // Define a largura do pulso, Largura do Pulso(ms) = OCRA/2000, 21ms
    //posição do servo f(x)=({x-1999}/2000-0.5)*270 p/ range de -135 a +135  ou ainda pos = ({x-bottom}/(top-bottom)-0.5)*range
    // f(x)= (x-2999)*0.135
    // f(x)=  (10x-29999)/74
    //para 270 graus de 1ms a 2ms cada 1 grau ~= 7,4 contas
    //para um angulo ang, o valor de OCRA1 deve ser ({ang+135}/135)/2*2000+2000

    uint8_t tecla;
    timer1.setCompareAValue(aux);

    uint8_t estado = 0;
    uint8_t subestado = 1;
    int16_t valor = 0;
    while(1) {
        switch(estado) {
        case 0:
            //entrando no modo ocioso
            lcd.displayStateSet(Hd44780::DisplayState::CURSOR_OFF);
            angulo = getangle(aux);
            lcd.clearScreen();
            printf("Angulo = %d\n", angulo);
            delayMs(50);
            while(estado == 0) {
                angulo = getangle(aux);
                keypad.readKeyPressed(&tecla);
                switch(tecla) {
                case 0xFF:
                    break;

                case '0':
                    aux = 2999;
                    OCR1A = aux;
                    angulo = getangle(aux);
                    lcd.clearScreen();
                    printf("Angulo = %d\n", angulo);
                    delayMs(50);
                    break;

                case 'C':
                    aux = aux - 37;
                    if(aux < 1999) {
                        aux = 1999;
                    }
                    OCR1A = aux;
                    angulo = getangle(aux);
                    lcd.clearScreen();
                    printf("Angulo = %d\n", angulo);
                    delayMs(50);
                    break;

                case 'D':
                    aux = aux + 37;
                    if(aux > 3999) {
                        aux = 3999;
                    }
                    OCR1A = aux;
                    angulo = getangle(aux);
                    lcd.clearScreen();
                    printf("Angulo = %d\n", angulo);
                    delayMs(50);
                    break;

                case 'F':
                    estado = 1;
                    subestado = 1;
                    valor = 0;
                    break;
                default:
                    break;
                }
            }
        case 1:
            angulo = getangle(aux);
            lcd.clearScreen();
            printf("Angulo = %d \n", angulo);
            printf("Novo=");
            lcd.displayStateSet(Hd44780::DisplayState::BLINK_ON);
            lcd.cursorGoTo(2, 5);
            while(estado == 1) {
                keypad.readKeyPressed(&tecla);
                switch(tecla) {
                case 'C':
                    // printf("tecla %c pressionada", tecla);
                    // delayMs(300);
                    // lcd.clearScreen();
                    valor = -valor;
                    lcd.clearScreen();
                    printf("Angulo = %d \n", angulo);
                    printf("Novo= %d", valor);
                    break;
                case 'E':
                    estado = 0;
                    break;
                case 'F':
                    if(valor >= -135 && valor <= 135) {
                        setangle(valor);
                        estado = 0;
                    } else {
                        lcd.clearScreen();
                        lcd.displayStateSet(Hd44780::DisplayState::CURSOR_OFF);
                        printf("Angulo = %d \n", angulo);
                        printf("Valor invalido!");
                        delayMs(1500);
                        estado = 0;
                    }
                    break;
                case 0xFF:
                    break;
                default:
                    //lcd.clearScreen();
                    //printf("tecla %c pressionada", tecla);
                    //delayMs(300);
                    switch(subestado) {
                    case 1:
                        valor = typing(0, tecla - '0', subestado);
                        lcd.clearScreen();
                        printf("Angulo = %d \n", angulo);
                        printf("Novo= %d", valor);
                        subestado++;
                        break;
                    case 2:
                        valor = typing(valor, tecla - '0', subestado);
                        lcd.clearScreen();
                        printf("Angulo = %d \n", angulo);
                        printf("Novo= %d", valor);
                        subestado++;
                        break;
                    case 3:
                        valor = typing(valor, tecla - '0', subestado);
                        lcd.clearScreen();
                        printf("Angulo = %d \n", angulo);
                        printf("Novo= %d", valor);
                        subestado++;
                        break;
                    default:
                        valor = typing(valor, tecla - '0', subestado);
                        lcd.clearScreen();
                        printf("Angulo = %d \n", angulo);
                        printf("Novo= %d", valor);
                        subestado++;
                        break;
                    }
                    break;
                }
            }
        }
        //Início do modo ocioso










        //OCR1A = 35333; // Set position to minimum degrees, 1ms pulse width
        // _delay_ms(500);
        // aux = aux + 148;
        // if (aux > 42000){
        //     aux = 1999;
        // }
    }

    return 0;
}
