#include <Arduino.h>
#include <avr/io.h>

int main(void) {
    init();
    DDRL = 0xFF;
    PORTL = 0;

    while (1) {
        const uint16_t potentiometer = analogRead(A0);
        // Map the 10-bit ADC value (0..1023) to eight LEDs (0..255).
        PORTL = (uint8_t)(potentiometer / 4);
        delay(200);
    }
}
