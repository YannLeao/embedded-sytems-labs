#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

uint8_t count = 0;
uint16_t timer_counter = 0;

ISR(INT0_vect) {
  count++;
  PORTL = count;
}

ISR(TIMER0_OVF_vect) {
  TCNT0 = 6;
  timer_counter++;

  if (timer_counter >= 1000) {
    timer_counter = 0;
    PORTA ^= (1 << PA0);
  }

}

int main(void) {

  cli();

  DDRL = 0xFF;
  PORTL = 0x00;

  DDRA |= (1 << PA0);
  PORTA &= ~(1 << PA0);

  DDRD &= ~(1 << PD0);
  PORTD &= ~(1 << PD0);

  EICRA &= ~((1 << ISC01) | (1 << ISC00));
  EICRA |= (1 << ISC01);

  EIFR = (1 << INTF0);
  EIMSK |= (1 << INT0);

  TCCR0B = 0;
  TCCR0A = 0;
  TCNT0 = 6;

  TIFR0 = (1 << TOV0);
  TIMSK0 = (1 << TOIE0);

  TCCR0B = (1 << CS01) | (1 << CS00);

  sei();

  while (1) {

  }
}