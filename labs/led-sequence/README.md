# Lab 02: LED sequence

Reconstructed from lab 02. Source: [main.c](main.c). Guide: [lab-02-led-sequence.pdf](../../docs/lab-guides/lab-02-led-sequence.pdf).

## Hardware conventions

Target: ATmega2560 at 16 MHz on an Arduino Mega 2560. LEDs are active-high, each with its own series resistor to GND. PORTL bit 0..7 maps to Mega D49, D48, D47, D46, D45, D44, D43, D42. Wire physical LED order explicitly; Arduino digital numbering does not follow PORTL bit order.


## Wiring for this checkpoint

Eight LEDs on PORTL, ordered PL0 at the right through PL7 at the left.

## Expected behavior

Each LED stays on as the next LED lights, with 500 ms between activations. Once all eight are on, hold for 1 s, clear them for 1 s and repeat.

## Study notes

Observe the distinction between `PORTL |= mask` (accumulate) and `PORTL = mask` (replace). The full-on hold starts immediately after the final activation.

## Build and check on the board

```sh
pio run -e led-sequence
pio run -e led-sequence -t upload
```

Compare the physical behavior with the description above. For serial examples, use `pio device monitor -b 9600`. Compilation status is recorded in [validation](../../docs/validation.md); these checkpoints have not yet been tested on physical hardware.
