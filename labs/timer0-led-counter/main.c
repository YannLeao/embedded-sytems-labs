#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

static uint8_t count = 0;
static uint16_t timer_counter = 0;

ISR(TIMER0_OVF_vect) {
    TCNT0 = 6;
    timer_counter++;
    if (timer_counter >= 1000) {
        timer_counter = 0;
        count++;
        PORTL = count;
    }
}

int main(void) {
    cli();
    DDRL = 0xFF;
    PORTL = 0;

    TCCR0B = 0; // Stop before configuring.
    TCCR0A = 0; // Normal mode.
    TCNT0 = 6;
    TIFR0 = (1 << TOV0);
    TIMSK0 = (1 << TOIE0);
    // 16 MHz / 64 = 250 kHz; (256 - 6) ticks = nominally 1 ms.
    TCCR0B = (1 << CS01) | (1 << CS00);

    sei();
    while (1) {}
}
