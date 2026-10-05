# Validation record

Updated on 2026-10-05 after successful PlatformIO setup on Fedora with CLion. The previous XC8 checks were performed in the supplied Linux workspace. No upload or physical-board validation has been recorded.

## Completed

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

- Ten environments selecting ten distinct, existing source files, with all other lab sources excluded.
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
- **Arduino integration:** build the four C++ environments against the actual Arduino AVR framework. Confirm that their explicit `main()` entry points link correctly; the servo environment also requires Servo 1.2.2.
- **Upload and serial access:** confirm USB upload, Fedora device permissions and the 9600-baud monitor for the serial examples.
- **Physical behavior:** check LED order, sensor idle level and falling-edge detection, counter rollover, timer intervals, servo motion and ADC threshold behavior. Verify the moving average's startup response and compare multiple window sizes with a repeatable potentiometer input.
- **Debugging:** no new simulator or hardware breakpoint session has been validated with this PlatformIO/CLion setup.

Build the remaining environments explicitly; plain `pio run` selects only the default environment:

```sh
pio run -e led-sequence -e led-bounce -e led-odd-even-sequence -e external-interrupt-counter -e timer0-led-counter -e potentiometer-led-output -e servo-external-interrupt-counter -e adc-threshold -e adc-moving-average
```

Use the Mega wiring described in each exercise, including when guide illustrations show an Uno. A generated ELF or HEX does not by itself confirm upload or physical operation.

Git initialization, a local commit and GitHub publication remain separate future steps. The workspace's `.git` directory is still read-only and no initialized project repository was found during this review. The Git ignore rules are ready; instructions are in [setup](setup.md).
