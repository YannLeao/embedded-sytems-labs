# Lab 02 challenge: bouncing LED

Reconstructed from the lab 02 challenge. Source: [main.c](main.c). Guide: [lab-02-led-sequence.pdf](../../docs/lab-guides/lab-02-led-sequence.pdf).

## Hardware conventions

Target: ATmega2560 at 16 MHz on an Arduino Mega 2560. LEDs are active-high, each with its own series resistor to GND. PORTL bit 0..7 maps to Mega D49, D48, D47, D46, D45, D44, D43, D42. Wire physical LED order explicitly; Arduino digital numbering does not follow PORTL bit order.


## Wiring for this checkpoint

Eight LEDs on PORTL, PL0 at the right and PL7 at the left.

## Expected behavior

Exactly one LED lights for 500 ms at a time: 0,1,2,3,4,5,6,7,6,5,4,3,2,1, then repeat. Endpoints are displayed once per turn.

## Study notes

Assignment replaces the entire port. The descending index is signed to avoid unsigned wraparound.

## Build and check on the board

```sh
pio run -e led-bounce
pio run -e led-bounce -t upload
```

Compare the physical behavior with the description above. For serial examples, use `pio device monitor -b 9600`. Compilation status is recorded in [validation](../../docs/validation.md); these checkpoints have not yet been tested on physical hardware.
