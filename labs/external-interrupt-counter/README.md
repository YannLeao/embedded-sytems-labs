# Lab 04: external interrupt counter

Reconstructed from lab 04. Source: [main.c](main.c). Guide: [lab-04-infrared-external-interrupt.pdf](../../docs/lab-guides/lab-04-infrared-external-interrupt.pdf).

## Hardware conventions

Target: ATmega2560 at 16 MHz on an Arduino Mega 2560. LEDs are active-high, each with its own series resistor to GND. PORTL bit 0..7 maps to Mega D49, D48, D47, D46, D45, D44, D43, D42. Wire physical LED order explicitly; Arduino digital numbering does not follow PORTL bit order.


## Wiring for this checkpoint

Eight LEDs on PORTL. Sensor output to PD0/INT0, Mega D21, with a defined idle-high level supplied by the sensor circuit; common GND. The internal pull-up is disabled, following the supplied register style.

## Expected behavior

Each falling edge increments the 8-bit counter. LEDs display its binary value; 255 wraps to 0.

## Study notes

INT0_vect refers to the hardware vector on D21. Arduino attachInterrupt index 0 has a different board-specific mapping; this code does not use that API. Do not leave PD0 floating or use D21 for I2C simultaneously. Count events, not necessarily physical objects when the sensor produces extra edges.

## Build and check on the board

```sh
pio run -e external-interrupt-counter
pio run -e external-interrupt-counter -t upload
```

Compare the physical behavior with the description above. For serial examples, use `pio device monitor -b 9600`. Compilation status is recorded in [validation](../../docs/validation.md); these checkpoints have not yet been tested on physical hardware.
