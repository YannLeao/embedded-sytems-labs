#include <Arduino.h>
#include <avr/io.h>

int main(void) {
    init();
    Serial.begin(9600);
    DDRL |= (1 << PL0);
    PORTL &= ~(1 << PL0);

    while (1) {
        const uint16_t sample = analogRead(A0);
        if (sample > 500) {
            PORTL |= (1 << PL0);
        } else {
            PORTL &= ~(1 << PL0);
        }
        Serial.println(sample);
        delay(100);
    }
}
