#include <Arduino.h>
#include <Wire.h>
#include <util/atomic.h>

#ifndef I2C_SLAVE_ADDRESS
#define I2C_SLAVE_ADDRESS 17
#endif

static_assert(I2C_SLAVE_ADDRESS >= 0x08 && I2C_SLAVE_ADDRESS <= 0x77,
              "Use a non-reserved 7-bit I2C address (0x08..0x77)");

static const uint8_t packet_capacity = 32; // Wire's AVR receive buffer size.
static volatile uint8_t received_packet[packet_capacity];
static volatile uint8_t received_size = 0;

// Called by Wire inside the TWI ISR. Keep Serial and delays out of this function.
static void receive_event(int byte_count) {
    if (byte_count < 2 || byte_count > packet_capacity) {
        while (Wire.available()) {
            Wire.read();
        }
        return;
    }

    uint8_t size = 0;
    while (Wire.available() && size < packet_capacity) {
        received_packet[size++] = static_cast<uint8_t>(Wire.read());
    }
    // A single-slot mailbox: a newer message replaces any unread message.
    received_size = size;
}

int main(void) {
    init();
    Serial.begin(9600);
    Wire.onReceive(receive_event);
    Wire.begin(static_cast<uint8_t>(I2C_SLAVE_ADDRESS));

    while (1) {
        uint8_t packet[packet_capacity];
        uint8_t size;
        // volatile alone cannot protect a whole packet against an ISR update.
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            size = received_size;
            for (uint8_t i = 0; i < size; i++) {
                packet[i] = received_packet[i];
            }
            received_size = 0;
        }

        if (size >= 2) {
            Serial.write(packet, size - 1); // Text prefix, without a terminator.
            Serial.println(packet[size - 1]); // Last byte displayed as decimal.
        }
    }
}
