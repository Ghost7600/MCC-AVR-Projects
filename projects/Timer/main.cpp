
#include <avr/io.h>
#include "funsape/globalDefines.hpp"
#include "timer1.hpp"

int main()
{
    uint8_t hour = 12;
    uint8_t minutes = 34;
    uint8_t seconds = 56;

    vbool_t changetTime = false;

    Hd44780 display;

    //timer1 configuration
    timer1.init();
    timer1.setCompareAValue();
    timer1.activateCompareAInterrupt();

    //LCD initialization
    display.controlPortSet();
    display.dataPortSet();
    display.init(
            Hd44780::Size::LCD_16x2,
            Hd44780::FONT_5X8,
            true,
            false
    );
    display.displayStateSet(Hd44780::DisplayState::CURSOR_OFF);
    display.stdio();
    sei();
    while(1) {
        if(changetime) {
            // (((seconds==59) ? (seconds==0)) ? ((minutes==59)?(minutes==0))?(hours==12));
            if(seconds == 59) {
                seconds = 0;
                if(minutes == 59) {
                    minutes = 0;
                    if(hour = 23) {
                        hours = 0;
                    }
                }
            }
            //Update Display
            printf(" %02u/%02u/20%02u/ %s \n", 18, 4, 23, "TER");
            printf("   %02u:%02u:%02u   \n", hours, minutes, seconds);

            //clear flags
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
