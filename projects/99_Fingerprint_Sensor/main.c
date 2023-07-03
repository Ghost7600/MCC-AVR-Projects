
#include <avr/io.h>
#include "funsape/peripheral/usart0.hpp"
int main()
{
    uint8_t hour = 12;
    uint8_t minutes = 34;
    uint8_t seconds = 56;

    vbool_t changetTime = false;

    //timer1 configuration
    // timer1.init();
    // timer1.setCompareAValue();
    // timer1.activateCompareAInterrupt();

    while(1) {
        if(changetime) {
            (seconds = 59) ? 0;
        }
    }

    return 0;
}
