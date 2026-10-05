# Lab 06: servo and external interrupt counter

Adapted from the original pratica-07.c; fixed its missing semicolon and translated identifiers. Source: [main.cpp](main.cpp). Guide: [lab-06-servo-motor.pdf](../../docs/lab-guides/lab-06-servo-motor.pdf).

## Hardware conventions

Target: ATmega2560 at 16 MHz on an Arduino Mega 2560. LEDs are active-high, each with its own series resistor to GND. PORTL bit 0..7 maps to Mega D49, D48, D47, D46, D45, D44, D43, D42. Wire physical LED order explicitly; Arduino digital numbering does not follow PORTL bit order.


## Wiring for this checkpoint

Servo signal to D9; sensor output to PD0/INT0/D21 with defined idle-high level and common GND. Eight LEDs on PORTL. Use a suitable servo supply and common ground with the Mega.

## Expected behavior

Sweep 0 to 180 degrees and back, in 1-degree commands with 10 ms delays; pause 100 ms after the return. Every falling sensor edge increments the byte on PORTL.

## Study notes

The count is sensor events. One complete sweep may produce more than one event depending on where the beam is mounted. Servo uses Timer5 for the first servo on this board; Arduino keeps Timer0 for delays. No PWM I/O API replaces the register-based LED outputs.

## Build and check on the board

```sh
pio run -e servo-external-interrupt-counter
pio run -e servo-external-interrupt-counter -t upload
```

Compare the physical behavior with the description above. For serial examples, use `pio device monitor -b 9600`. Compilation status is recorded in [validation](../../docs/validation.md); these checkpoints have not yet been tested on physical hardware.
