#include <Arduino.h>
#include <avr/io.h>

#ifndef ADC_WINDOW_SIZE
#define ADC_WINDOW_SIZE 10
#endif

static_assert(ADC_WINDOW_SIZE >= 1 && ADC_WINDOW_SIZE <= 64,
              "Use a window size from 1 to 64 samples");
static uint16_t samples[ADC_WINDOW_SIZE] = {};
static uint32_t sum = 0;
static uint8_t next_sample = 0;
static uint8_t sample_count = 0;

int main(void) {
    init();
    Serial.begin(9600);
    Serial.println("raw,average");

    while (1) {
        const uint16_t sample = analogRead(A0);
        sum -= samples[next_sample];
        samples[next_sample] = sample;
        sum += sample;
        next_sample = (next_sample + 1) % ADC_WINDOW_SIZE;
        if (sample_count < ADC_WINDOW_SIZE) {
            sample_count++;
        }
        // During startup average only the samples actually collected.
        const uint16_t average = sum / sample_count;
        Serial.print(sample);
        Serial.print(',');
        Serial.println(average);
        delay(100);
    }
}
