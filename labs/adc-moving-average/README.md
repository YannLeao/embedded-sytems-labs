# Lab 07: moving-average filter

Reconstructed from the moving-average section of lab 07; window and sample interval are explicit checkpoint choices. Source: [main.cpp](main.cpp). Guide: [lab-07-adc.pdf](../../docs/lab-guides/lab-07-adc.pdf).

## Hardware conventions

Target: ATmega2560 at 16 MHz on an Arduino Mega 2560. LEDs are active-high, each with its own series resistor to GND. PORTL bit 0..7 maps to Mega D49, D48, D47, D46, D45, D44, D43, D42. Wire physical LED order explicitly; Arduino digital numbering does not follow PORTL bit order.


## Wiring for this checkpoint

A0/PF0 with an unconnected jumper for the guide’s floating-input noise observation, or a potentiometer for a repeatable response test. Serial monitor at 9600 baud. A floating input is intentional only for this experiment.

## Expected behavior

Print raw,average CSV pairs every 100 ms, preceded by a header. Default window: 10 samples. Change ADC_WINDOW_SIZE in platformio.ini to compare 5, 10 and 20.

## Study notes

A ring buffer replaces the oldest value and maintains a running sum. Startup averages only collected samples, avoiding artificial zero padding. The integer average truncates fractions. A 32-bit sum accommodates the configured window safely. Larger windows add lag; a floating input does not measure a defined voltage.

## Build and check on the board

```sh
pio run -e adc-moving-average
pio run -e adc-moving-average -t upload
```

Compare the physical behavior with the description above. For serial examples, use `pio device monitor -b 9600`. Compilation status is recorded in [validation](../../docs/validation.md); these checkpoints have not yet been tested on physical hardware.
