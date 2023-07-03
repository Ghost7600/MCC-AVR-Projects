#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../funsape/globalDefines.hpp"
#include "../funsape/peripheral/int0.hpp"
#include "../funsape/peripheral/timer0.hpp"




void usartsendbyte(uint8_t byte);

void usartsendstring (char string[64]);

void signalbegin();
