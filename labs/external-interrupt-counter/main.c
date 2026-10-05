#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

static uint8_t count = 0; // Used only by the ISR, not shared with main.

ISR(INT0_vect) {
    count++;
    PORTL = count;
}

int main(void) {
    cli();
    DDRL = 0xFF;
    PORTL = 0;

    // INT0 is PD0 / Mega D21. The sensor circuit supplies the idle level.
    DDRD &= ~(1 << PD0);
    PORTD &= ~(1 << PD0);
    EICRA &= ~((1 << ISC01) | (1 << ISC00));
    EICRA |= (1 << ISC01); // Falling edge.
    EIFR = (1 << INTF0);   // Clear pending flag by writing one.
    EIMSK |= (1 << INT0);

    sei();
    while (1) {}
}
