#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

uint8_t count = 0;
int count_timer = 0;

ISR(TIMER0_OVF_vect) {
  TCNT0 = 6;
  count_timer++;

  if (count_timer >= 1000) {
    count_timer = 0;
    count++;
    PORTL = count;
  }
}

int main(void)
{
  cli();
  
  DDRL = 0xFF;
  PORTL = 0x00;
  
  TCCR0A = 0;
  TCCR0B = (1 << CS01) | (1 << CS00);
  TCNT0 = 6;
  TIMSK0 = (1 << TOIE0);
  
  sei();
  
  while (1) {}
}
