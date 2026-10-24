#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>

struct Slave {
    uint8_t address;
    const char *label;
};

static const Slave slaves[] = {
    {25, "x = "},
    {17, "x1 = "},
    {26, "x2 = "},
};

int main(void) {
    init();
    Serial.begin(9600);
    Wire.begin();
    Wire.setClock(100000UL);
    Wire.setWireTimeout(25000UL, true);

    uint8_t value = 0;
    while (1) {
        for (const Slave &slave : slaves) {
            Wire.beginTransmission(slave.address);
            Wire.write(slave.label);
            Wire.write(value); // One binary byte, not its decimal text.
            const uint8_t status = Wire.endTransmission();
            if (status != 0) {
                Serial.print("I2C address ");
                Serial.print(slave.address);
                Serial.print(": error ");
                Serial.println(status);
            }
        }
        // The original three counters advanced together, so one is enough.
        value++; // uint8_t wraps from 255 to 0.
        delay(500);
    }
}
