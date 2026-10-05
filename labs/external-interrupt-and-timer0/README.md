# Combined checkpoint: INT0 and Timer0

Adapted from the original main.c. Source: [main.c](main.c). Guide: [lab-10-timer0-overflow-interrupt.pdf](../../docs/lab-guides/lab-10-timer0-overflow-interrupt.pdf).

## Hardware conventions

Target: ATmega2560 at 16 MHz on an Arduino Mega 2560. LEDs are active-high, each with its own series resistor to GND. PORTL bit 0..7 maps to Mega D49, D48, D47, D46, D45, D44, D43, D42. Wire physical LED order explicitly; Arduino digital numbering does not follow PORTL bit order.


## Wiring for this checkpoint

Eight LEDs on PORTL, an additional LED on PA0/D22, and sensor output to PD0/INT0/D21 with a defined idle-high signal from the external circuit and common GND.

## Expected behavior

Falling INT0 edges increment the PORTL byte. Timer0 counts 1,000 nominal 1 ms overflows and toggles PA0. The LED changes state roughly every second, with a roughly two-second full cycle.

## Study notes

The interrupt handlers execute serially; the main loop is empty. The counters are accessed only inside their ISRs, so they do not need volatile for foreground communication. If main later reads ISR state, use volatile and protect multi-byte accesses appropriately. This is a bare AVR build, not an Arduino Timer0 configuration.

## Build and check on the board

```sh
pio run -e external-interrupt-and-timer0
pio run -e external-interrupt-and-timer0 -t upload
```

Compare the physical behavior with the description above. For serial examples, use `pio device monitor -b 9600`. Compilation status is recorded in [validation](../../docs/validation.md); these checkpoints have not yet been tested on physical hardware.
