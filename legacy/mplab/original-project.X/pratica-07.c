#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include <Servo.h>

volatile uint8_t contador = 0;
Servo servo;

ISR(INT0_vect) {
    contador++;
    PORTL = contador;
}

int main(void)
{
  init();
  
  cli();
  
  PORTL = 0;
  DDRL = 0xFF;
  
  DDRD &= ~(1 << PD0)
  
  EICRA &= ~((1 << ISC01) | (1 << ISC00));
  EICRA |=  (1 << ISC01);
  
  EIFR = (1 << INTF0);
  EIMSK |= (1 << INT0);
  
  servo.attach(9);
  servo.write(0);
  
  sei();

  while (1) {
      
    for (int theta = 0; theta <= 180; theta++) {
        servo.write(theta);
        delay(10);
    }

    for (int theta = 180; theta >= 0; theta--) {
        servo.write(theta);
        delay(10);
    }

    delay(100);
    }
}
