#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

int main(void) {
    DDRL = 0xFF;
    PORTL = 0;

    while (1) {
        for (uint8_t bit = 0; bit < 8; bit++) {
            PORTL |= (1 << bit);
            // Seven gaps between LEDs; the final LED starts the full-on hold.
            if (bit < 7) {
                _delay_ms(500);
            }
        }
        _delay_ms(1000);
        PORTL = 0;
        _delay_ms(1000);
    }
}
