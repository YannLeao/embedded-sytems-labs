# Lab 02 part 2: odd/even LED sequence

Reconstructed from lab 02 part 2 and adapted to ten Mega outputs. Source: [main.c](main.c). Guide: [lab-02-led-sequence-part-02.pdf](../../docs/lab-guides/lab-02-led-sequence-part-02.pdf).

## Hardware conventions

Target: ATmega2560 at 16 MHz on an Arduino Mega 2560. LEDs are active-high, each with its own series resistor to GND. PORTL bit 0..7 maps to Mega D49, D48, D47, D46, D45, D44, D43, D42. Wire physical LED order explicitly; Arduino digital numbering does not follow PORTL bit order.


## Wiring for this checkpoint

Physical positions 1..8 (right to left) are PL0..PL7 (D49..D42). Position 9 is PA0/D22 and position 10 is PA1/D23.

## Expected behavior

Accumulate positions 1,3,5,7,9 from right to left at 500 ms intervals; hold 1 s and clear for 1 s. Then accumulate positions 10,8,6,4,2 from left to right; hold 1 s and clear for 1 s.

## Study notes

Human positions are numbered from 1; zero-based bit indices have opposite parity. Only PA0 and PA1 are changed. The guide repeats “odd” in the even-phase hold description; this checkpoint interprets it as holding the even LEDs just lit.

## Build and check on the board

```sh
pio run -e led-odd-even-sequence
pio run -e led-odd-even-sequence -t upload
```

Compare the physical behavior with the description above. For serial examples, use `pio device monitor -b 9600`. Compilation status is recorded in [validation](../../docs/validation.md); these checkpoints have not yet been tested on physical hardware.
