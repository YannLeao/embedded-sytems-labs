#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

// Position 1 (rightmost) is PL0; positions 9 and 10 are PA0 and PA1.
static void light_position(uint8_t position) {
    if (position < 8) {
        PORTL |= (1 << position);
    } else {
        PORTA |= (1 << (position - 8));
    }
}

static void clear_leds(void) {
    PORTL = 0;
    PORTA &= ~((1 << PA0) | (1 << PA1));
}

int main(void) {
    DDRL = 0xFF;
    DDRA |= (1 << PA0) | (1 << PA1);
    clear_leds();

    while (1) {
        // Human positions 1, 3, 5, 7, 9: right to left.
        for (uint8_t position = 0; position <= 8; position += 2) {
            light_position(position);
            if (position < 8) {
                _delay_ms(500);
            }
        }
        _delay_ms(1000);
        clear_leds();
        _delay_ms(1000);

        // Human positions 10, 8, 6, 4, 2: left to right.
        for (int8_t position = 9; position >= 1; position -= 2) {
            light_position((uint8_t)position);
            if (position > 1) {
                _delay_ms(500);
            }
        }
        _delay_ms(1000);
        clear_leds();
        _delay_ms(1000);
    }
}
