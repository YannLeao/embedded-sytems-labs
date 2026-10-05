# Lab 07: ADC threshold and calibration

Reconstructed from the potentiometer/calibration section of lab 07. Source: [main.cpp](main.cpp). Guide: [lab-07-adc.pdf](../../docs/lab-guides/lab-07-adc.pdf).

## Hardware conventions

Target: ATmega2560 at 16 MHz on an Arduino Mega 2560. LEDs are active-high, each with its own series resistor to GND. PORTL bit 0..7 maps to Mega D49, D48, D47, D46, D45, D44, D43, D42. Wire physical LED order explicitly; Arduino digital numbering does not follow PORTL bit order.


## Wiring for this checkpoint

Potentiometer outer terminals to 5 V and GND, wiper to A0/PF0. One LED on PL0/D49. Serial monitor at 9600 baud.

## Expected behavior

Print a raw ADC sample every 100 ms. Values 0..500 turn the LED off; values 501..1023 turn it on.

## Study notes

The default analog reference follows the board AVcc supply. Observe values near 500 and verify the strict greater-than threshold. There is no hysteresis, so noise near the threshold can flicker the LED.

## Build and check on the board

```sh
pio run -e adc-threshold
pio run -e adc-threshold -t upload
```

Compare the physical behavior with the description above. For serial examples, use `pio device monitor -b 9600`. Compilation status is recorded in [validation](../../docs/validation.md); these checkpoints have not yet been tested on physical hardware.
