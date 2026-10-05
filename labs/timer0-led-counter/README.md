# Lab 10: Timer0 LED counter

Adapted from the original pratica-05.c. Source: [main.c](main.c). Guide: [lab-10-timer0-overflow-interrupt.pdf](../../docs/lab-guides/lab-10-timer0-overflow-interrupt.pdf).

## Hardware conventions

Target: ATmega2560 at 16 MHz on an Arduino Mega 2560. LEDs are active-high, each with its own series resistor to GND. PORTL bit 0..7 maps to Mega D49, D48, D47, D46, D45, D44, D43, D42. Wire physical LED order explicitly; Arduino digital numbering does not follow PORTL bit order.


## Wiring for this checkpoint

Eight LEDs on PORTL.

## Expected behavior

Timer0 normal-mode overflow reloads TCNT0 to 6. Prescaler 64 at 16 MHz yields a nominal 1 ms overflow interval. After 1,000 overflows, increment the LED byte.

## Study notes

This is bare AVR: no Arduino init(), delay() or Timer0 framework ISR. The classroom software-reload technique gives approximate timing because interrupt/reload latency adds error. Counter rollover is intentional.

## Build and check on the board

```sh
pio run -e timer0-led-counter
pio run -e timer0-led-counter -t upload
```

Compare the physical behavior with the description above. For serial examples, use `pio device monitor -b 9600`. Compilation status is recorded in [validation](../../docs/validation.md); these checkpoints have not yet been tested on physical hardware.
