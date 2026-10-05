# Checkpoint: potentiometer to LED byte

Adapted from the original pratica-06.c; this is not the entire lab 07 assignment. Source: [main.cpp](main.cpp). Guide: [lab-07-adc.pdf](../../docs/lab-guides/lab-07-adc.pdf).

## Hardware conventions

Target: ATmega2560 at 16 MHz on an Arduino Mega 2560. LEDs are active-high, each with its own series resistor to GND. PORTL bit 0..7 maps to Mega D49, D48, D47, D46, D45, D44, D43, D42. Wire physical LED order explicitly; Arduino digital numbering does not follow PORTL bit order.


## Wiring for this checkpoint

Potentiometer outer terminals to 5 V and GND, wiper to A0/PF0. Eight LEDs on PORTL.

## Expected behavior

Read the 10-bit ADC value, divide by 4 and display the resulting byte every 200 ms. Midrange values are binary patterns, not a progressive LED bar.

## Study notes

Arduino init() initializes the ADC and time base. Keep Timer0 available for delay().

## Build and check on the board

```sh
pio run -e potentiometer-led-output
pio run -e potentiometer-led-output -t upload
```

Compare the physical behavior with the description above. For serial examples, use `pio device monitor -b 9600`. Compilation status is recorded in [validation](../../docs/validation.md); these checkpoints have not yet been tested on physical hardware.
