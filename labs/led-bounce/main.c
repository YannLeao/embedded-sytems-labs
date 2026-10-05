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
            PORTL = (1 << bit);
            _delay_ms(500);
        }
        // Skip endpoints already displayed by the outward pass.
        for (int8_t bit = 6; bit > 0; bit--) {
            PORTL = (1 << bit);
            _delay_ms(500);
        }
    }
}
