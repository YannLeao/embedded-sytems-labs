# Validation record

Updated on 2026-10-24 after building the I2C firmware. Earlier setup and XC8 checks are retained below. No upload or physical-board validation of the reorganized firmware has been recorded; the original I2C classroom experiment was performed on hardware, as reported by the author.

## I2C validation — 2026-10-24

All four new environments compiled and linked successfully with PlatformIO Core 6.1.19, Atmel AVR platform 5.1.0, framework-arduino-avr 5.2.0, bundled Wire 1.0 and AVR GCC 7.3.0:

```sh
pio run -e i2c-master -e i2c-slave -e i2c-slave-25 -e i2c-slave-26
```

| Environment | Compile/link + ELF/HEX | Flash | Static RAM | Hardware |
| --- | --- | --- | --- | --- |
| i2c-master | Passed | 4,592 bytes | 452 bytes | Pending |
| i2c-slave (17) | Passed | 3,868 bytes | 405 bytes | Pending |
| i2c-slave-25 | Passed | 3,868 bytes | 405 bytes | Pending |
| i2c-slave-26 | Passed | 3,868 bytes | 405 bytes | Pending |

No warnings came from the lab sources. The Arduino core's `new.cpp` emitted unused-parameter warnings under `-Wextra`. ELF symbol inspection confirmed one `main` and no `setup`/`loop` symbols in each firmware, plus the TWI interrupt handler and the slave receive callback. Static RAM figures do not include stack usage.

Structural checks confirmed one explicit `main()` and no `setup()`/`loop()` definitions in every active C/C++ source. Local links in the root README and I2C README resolve, and `git diff --check` passed. The new environments reuse the same slave source with distinct address definitions; they are four firmware configurations, not four distinct source files.

The installed Wire implementation was inspected to confirm receive callbacks run in the TWI ISR and to check buffer and timeout behavior. The classroom address 3 was replaced with non-reserved address 26; serial output was moved out of the callback, and shared packet copying uses an atomic block.

Physical checks remain: wiring/pull-ups, unique addresses, serial output and rollover, missing-slave diagnosis, timeout behavior, oscilloscope decoding, and the optional six-slave configuration. See the [I2C bench checklist](../labs/i2c-protocol/README.md#conferência-na-bancada).

## Earlier checks — 2026-10-05

### PlatformIO and CLion on Fedora

PlatformIO Core **6.1.19** is installed in `~/.venvs/embedded-labs`. The `pio` command is available on `PATH` through `~/.local/bin/pio`, linked to the virtual environment executable. The version and link were also inspected during this documentation review.

The successful CLion recovery flow was to close the IDE, recreate the old `.idea/` state, and open `platformio.ini` specifically with **Open as Project**. The PlatformIO tool window and Build/Upload/Reload actions confirm the project integration, as reported in the working setup.

The **external-interrupt-and-timer0** environment compiled and linked successfully using PlatformIO Core 6.1.19:

```sh
pio run -e external-interrupt-and-timer0
```

The build generated both artifacts, whose presence was confirmed during this review:

- `.pio/build/external-interrupt-and-timer0/firmware.elf`
- `.pio/build/external-interrupt-and-timer0/firmware.hex`

The successful build was reported from the local Fedora setup; this documentation review checked the installed version and artifact presence without rerunning the build. This environment is bare AVR, so it does not validate the Arduino framework or Servo library integration.

### Earlier XC8 checks

All six bare AVR checkpoints compiled and linked successfully for **ATmega2560** using **Microchip XC8 3.10**, **ATmega DFP 3.6.299**, optimization `-O1` and warning flags `-Wall -Wextra`. No source warnings were reported. ELF symbol inspection of the combined checkpoint confirmed an application `main` and the INT0 and Timer0 overflow handlers.

| Checkpoint | XC8 compile and link | PlatformIO build | Physical hardware |
| --- | --- | --- | --- |
| led-sequence | Passed | Pending | Pending |
| led-bounce | Passed | Pending | Pending |
| led-odd-even-sequence | Passed | Pending | Pending |
| external-interrupt-counter | Passed | Pending | Pending |
| timer0-led-counter | Passed | Pending | Pending |
| external-interrupt-and-timer0 | Passed | Passed (Core 6.1.19; ELF + HEX) | Pending |
| potentiometer-led-output | Not applicable to this C-only check | Pending | Pending |
| servo-external-interrupt-counter | Not applicable to this C-only check | Pending | Pending |
| adc-threshold | Not applicable to this C-only check | Pending | Pending |
| adc-moving-average | Not applicable to this C-only check | Pending | Pending |

### Configuration and documentation

The configuration and documentation were also checked for:

- The original ten environments selecting ten distinct, existing source files, with all other lab sources excluded.
- Exactly one explicit `main()` per active source and no `setup()` or `loop()` definitions.
- Arduino environments using `.cpp` and calling `init()`; bare AVR environments using `.c` without Arduino initialization.
- Local Markdown links resolving to existing files.

Git ignore rules were checked with Git itself using temporary metadata under `/tmp`, without initializing this project's repository. `.idea/`, `.pio/`, build directories, generated firmware, logs, compilation databases, local virtual environments and private MPLAB configuration matched the current `.gitignore`. Active sources, documentation, `platformio.ini`, scripts and shared MPLAB project files remained eligible for version control. No ignore-rule changes were necessary.

These structural checks do not prove that the Arduino dependencies link correctly or that hardware behaves as intended.

## Reproduce the available XC8 check

If the original compiler and device pack are installed at the same paths:

```sh
./scripts/build-bare-avr.sh
```

On another installation, provide the actual paths:

```sh
XC8_CC=/path/to/xc8-cc ATMEGA_DFP=/path/to/ATmega_DFP/version ./scripts/build-bare-avr.sh
```

The optional script builds only `labs/*/main.c` and stores output in the ignored `build/xc8-check/` directory. It accepts an output directory as its first argument. The primary build configuration remains PlatformIO.

## Remaining verification

- **Other PlatformIO environments:** nine environments still lack a recorded successful PlatformIO build. The earlier XC8 results remain valid for the six C examples, but do not replace a PlatformIO check of each environment.
- **Earlier Arduino integration:** the original four C++ environments still need builds; the I2C builds above now validate explicit `main()` linking with Arduino AVR and Wire. The servo environment also requires Servo 1.2.2.
- **Upload and serial access:** confirm USB upload, Fedora device permissions and the 9600-baud monitor for the serial examples.
- **Physical behavior:** check LED order, sensor idle level and falling-edge detection, counter rollover, timer intervals, servo motion and ADC threshold behavior. Verify the moving average's startup response and compare multiple window sizes with a repeatable potentiometer input.
- **Debugging:** no new simulator or hardware breakpoint session has been validated with this PlatformIO/CLion setup.

Build the remaining environments explicitly; plain `pio run` selects only the default environment:

```sh
pio run -e led-sequence -e led-bounce -e led-odd-even-sequence -e external-interrupt-counter -e timer0-led-counter -e potentiometer-led-output -e servo-external-interrupt-counter -e adc-threshold -e adc-moving-average
```

Use the Mega wiring described in each exercise, including when guide illustrations show an Uno. A generated ELF or HEX does not by itself confirm upload or physical operation.

Git is now initialized: the 2026-10-24 review found existing staged I2C additions and preserved their staging. No commit, staging change or publication was performed in this review. The earlier Git setup notes describe the initial workspace state.
