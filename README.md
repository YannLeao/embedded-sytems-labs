# Embedded Systems Labs

Practical exercises from an embedded systems course, targeting the **ATmega2560 at 16 MHz** on an Arduino Mega 2560. Topics include register-based I/O, interrupts, timers, ADC, servo control and I2C communication.

The course uses MPLAB X for simulation and debugging concepts, and Arduino IDE for board uploads. This repository uses **PlatformIO with CLion**, with an independent build environment for each exercise.

All examples use `int main(void)`. Examples that depend on the Arduino framework call `init()` explicitly. Sources are ordinary `.c` or `.cpp` files, without `.ino`, `setup()` or `loop()`.

## Getting started

Open `platformio.ini` in CLion using **Open as Project**, then select an environment to build or upload. From the terminal:

```sh
pio run -e external-interrupt-and-timer0
pio run -e external-interrupt-and-timer0 -t upload
```

## Project files

- [labs/](labs/): exercises with their own source code, wiring and study notes.
- [docs/setup.md](docs/setup.md): Fedora, PlatformIO and CLion setup, upload and debugging details.
- [docs/validation.md](docs/validation.md): completed checks and remaining validation.
- [docs/lab-guides/](docs/lab-guides/): original course guides in Portuguese, with English filenames.
- [legacy/mplab/](legacy/mplab/): preserved original MPLAB project.

All active examples target ATmega2560, including exercises whose recycled guide illustrations mention Arduino Uno. Use the Mega pin mappings documented with each exercise.

## I2C lab

The [I2C exercise](labs/i2c-protocol/README.md) has separate master and slave firmware, explicit `main()` entry points, and a destination table for sending to several boards with a `for` loop.

```sh
pio run -e i2c-master -e i2c-slave -e i2c-slave-25 -e i2c-slave-26
```
