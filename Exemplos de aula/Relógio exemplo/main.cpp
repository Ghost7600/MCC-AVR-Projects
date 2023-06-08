//---Definições---//
#define F_CPU 16000000UL

//---Includes---//
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/int0.hpp"
#include "funsape/device/hd44780.hpp"
#include "funsape/peripheral/twi.hpp" // para gerenciar o periferico do I2C
#include "funsape/device/ds1307.hpp"  // RTC utilizado no projeto.
#include "funsape/util/dateTime.hpp"  // gerencia data e hora para a funsape

//---Definições LED---//
#define LED_DIR     DDRC
#define LED_OUT     PORTC
#define LED_BIT     PC0

//---Definições PINOS de dados LCD---//
#define LCD_DATA_DIR        DDRB
#define LCD_DATA_OUT        PORTB
#define LCD_DATA_FIRST      PB0

//---Definições PINOS de controle LCD---//
#define LCD_CONTROL_DIR     DDRB
#define LCD_CONTROL_OUT     PORTB
#define LCD_CONTROL_E       PB4
#define LCD_CONTROL_RS      PB5

typedef union {
    struct {
        bool_t updateDisplay        : 1;
        bool_t readRtc              : 1;
        uint8_t unusedBits          : 6;
    };
    uint8_t allFlags;

} systemFlags_t;

//Global variables
volatile systemFlags_t systemFlags;


int main()
{
    //Local variables
    Hd44780 lcd;
    uint8_t hour, minute, second, monthDay, month, weekDay;
    uint16_t year;
    Ds1307 rtc;
    uint8_t aux8;




    //Configure LED for test
    setBit(LED_DIR, LED_BIT);
    clrBit(LED_OUT, LED_BIT);

    //Configure LCD
    lcd.controlPortSet(&LCD_CONTROL_DIR, LCD_CONTROL_E, LCD_CONTROL_RS);
    lcd.dataPortSet(&LCD_DATA_DIR, LCD_DATA_FIRST);
    lcd.init(Hd44780::Size::LCD_16X2, Hd44780::Font::FONT_5X8, true, false);
    lcd.displayStateSet(Hd44780::DisplayState::CURSOR_OFF);
    lcd.stdio();

    //Show splash screen
    printf(" Clock Example! \n");
    printf(" V1.0 build 238 \n");
    delayMs(2000);
    lcd.clearScreen();
    systemFlags.updateDisplay = true;

    //Configure INT0
    int0.init(Int0::SenseMode::FALLING_EDGE);
    int0.activateInterrupt();
    setBit(PORTD, PD2);

    //Enable global interrupts
    sei();

    //Configure I2C
    twi.init();

    // RTC só pode ser configurado depois da interrupção global.
    //Connfigure RTC
    rtc.init(&twi);

    //Verifica se a hora é a mesma de quando desligou
    rtc.getRamData(27, &aux8, 1);
    // Se o valor for diferente de 3, vai atualizar a hora
    if(aux8 != 3) {
        //Atualiza a hora
        rtc.setDate(2023, 5, 30);
        rtc.setTime(9, 5, 00);
        aux8 = 3;
        rtc.setRamData(27, &aux8, 1);

    }
    // Seta a hora
    rtc.setDate(2023, 5, 30);
    rtc.setTime(9, 5, 00);
    //Gera um clock de 1Hz
    rtc.setSquareWaveGenerator(Ds1307::SquareWave::CLOCK_1_HZ);
    // Inicia o clock
    rtc.clockStart();


    while(1) {

        //Event machine core
        if(systemFlags.updateDisplay) {
            //update display
            lcd.cursorHome();
            lcd.stdio();
            printf(" %02u/%02u/%04u %03u \n", monthDay, month, year, weekDay);
            printf("    %02u:%02u:%02u    \n", hour, minute, second);
            //Clear system flag
            systemFlags.updateDisplay = false;
        }
        if(systemFlags.readRtc) {

            //Reads RTC
            rtc.getDate(&year, &month, &monthDay, &weekDay);
            rtc.getTime(&hour, &minute, &second, DateTime::TimeFormat::FORMAT_24_HOURS);
            //Atualiza o display
            systemFlags.updateDisplay = true;
            //Clear flag
            systemFlags.readRtc = false;
        }


    }
    return 0;
}


void int0InterruptCallback()
{
    systemFlags.readRtc = true;
}
