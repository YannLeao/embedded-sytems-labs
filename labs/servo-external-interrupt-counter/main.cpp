#include <Arduino.h>
#include <avr/io.h>

#include <avr/interrupt.h>
#include <Servo.h>

static uint8_t count = 0;
static Servo servo;

ISR(INT0_vect) {
    count++;
    PORTL = count;
}

int main(void) {
    init();
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

    servo.attach(9);
    servo.write(0);
    sei();

    while (1) {
        for (int angle = 0; angle <= 180; angle++) {
            servo.write(angle);
            delay(10);
        }
        for (int angle = 180; angle >= 0; angle--) {
            servo.write(angle);
            delay(10);
        }
        delay(100);
    }
}
