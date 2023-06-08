#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB |= (1 << DDB5); // Set Pin 13 (PB5) as output

    while (1)
    {
        PORTB ^= (1 << PB5); // Toggle Pin 13 (PB5)
        _delay_ms(5000); // Delay for 500ms
    }

    return 0;
}
