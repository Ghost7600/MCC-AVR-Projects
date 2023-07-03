#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/int0.hpp"
#include "funsape/peripheral/timer0.hpp"

void bitbang(uint8_t tecla);
void sendstring (char string[64]);
