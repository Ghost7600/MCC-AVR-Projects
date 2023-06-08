//---Definições---//
#define F_CPU 16000000UL

//---Includes---//
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/timer1.hpp"
#include "funsape/device/hd44780.hpp"

vbool_t changeTime;

int main()
{
    //variáveis locais
    uint8_t hora = 12;
    uint8_t minuto = 34;
    uint8_t segundo = 56;
    Hd44780 lcd;

    changeTime = false;

    //Time 1 inicialização
    timer1.init(Timer1::Mode::CTC_OCRA, Timer1::ClockSource::PRESCALER_1024);
    timer1.setCompareAValue(15625);
    timer1.activateCompareAInterrupt();

    lcd.controlPortSet(&DDRB, PB4, PB4);
    lcd.dataPortSet(&DDRB, PB0);
    lcd.init(Hd44780::Size::LCD_16X2, Hd44780::Font::FONT_5X8, true, false);
    lcd.displayStateSet(Hd44780::DisplayState::CURSOR_OFF);
    lcd.stdio();

    sei();

    while(1) {
        // Trocar a hora do display
        if(changeTime) {
            //Calcular novo horário
            if(59 == segundo) {
                segundo = 0;
                if(59 == minuto) {
                    minuto = 0;
                    if(23 == hora) {
                        hora = 0;
                    } else {
                        hora++;
                    }
                } else {
                    minuto++;
                }
            } else {
                segundo++;
            }
            //Update display
            printf("%u/%u/%u \n", 18, 04, 2023);
            printf("    %02u:%02u:%02u    \n", hora, minuto, segundo);
            //Limpar flag
            changeTime = false;
        }
    }

    return 0;
}

void timer1CompareACallback(void)
{

    changeTime = true;
    return;
}
