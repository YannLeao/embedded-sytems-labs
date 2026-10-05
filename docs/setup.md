# Development setup

## The pieces of an embedded project

CLion is the editor and IDE. PlatformIO is the build/dependency/upload manager. The AVR toolchain compiles for the ATmega2560, rather than for the Linux computer. The Arduino framework supplies selected runtime functions and libraries. The Mega bootloader receives firmware through USB; uploading is not a breakpoint debugging session.

`avr/io.h` comes from the AVR toolchain, `Arduino.h` from the Arduino AVR framework, and `Servo.h` from a separate library. Missing headers or unknown registers in CLion often mean the project model has not loaded the appropriate compiler, include paths or target definitions. A missing semicolon is still a source error.

This project pins Atmel AVR platform 5.1.0 and Servo 1.2.2. Let PlatformIO resolve the platform's compatible framework/toolchain packages. Upgrade deliberately and rebuild the checkpoints afterwards.

## PlatformIO Core on Fedora

The working Fedora setup uses **PlatformIO Core 6.1.19** installed in the Python virtual environment `~/.venvs/embedded-labs`. Install Python with virtual-environment support, then install Core:

```sh
python3 -m venv "$HOME/.venvs/embedded-labs"
"$HOME/.venvs/embedded-labs/bin/python" -m pip install 'platformio==6.1.19'
"$HOME/.venvs/embedded-labs/bin/pio" --version
```

Make `pio` available on `PATH`. In the working setup, `~/.local/bin/pio` is a symbolic link to the virtual environment's executable:

```sh
mkdir -p "$HOME/.local/bin"
ln -s "$HOME/.venvs/embedded-labs/bin/pio" "$HOME/.local/bin/pio"
```

Create the link once; if it already exists, check its target with `ls -l "$HOME/.local/bin/pio"`. Ensure `~/.local/bin` is on `PATH`. For a Bash session:

```sh
export PATH="$HOME/.local/bin:$PATH"
command -v pio
pio --version
```

The expected results are `~/.local/bin/pio` (expanded to its absolute path) and `PlatformIO Core, version 6.1.19`. If needed, add the export to `~/.bashrc` and start a new shell. CLion must also inherit this `PATH`; restart it after changing the environment, or launch it from the configured terminal. With the link available on `PATH`, activating the virtual environment is not required for every build.

PlatformIO downloads AVR packages on the first build and requires network access. The earlier download failure in the restricted agent environment was resolved by this local Fedora setup.

## Open the project in CLion

Check that the PlatformIO integration is enabled in CLion. Open **the `platformio.ini` file specifically** through **File > Open**, and choose **Open as Project**.

For an existing CLion project that still opens as the old project type, the successful recovery flow was:

1. Close CLion completely.
2. Remove the project's old `.idea/` directory, or move it to a backup outside the project if you want to preserve local settings.
3. Launch CLion with `pio` available on its `PATH`.
4. Select this project's `platformio.ini` and choose **Open as Project**. CLion recreates `.idea/` for the PlatformIO project.
5. Confirm that the **PlatformIO tool window** and the **Build**, **Upload** and **Reload PlatformIO Project** actions appear. Select the desired environment for Build/Upload.

After changing environments or dependencies, use **Tools > PlatformIO > Reload PlatformIO Project**. If indexing remains stale, first confirm that `pio run -e <environment>` builds in the terminal, then reload the project. Menu wording can vary by CLion version.

The setup was confirmed with a successful build of the default environment:

```sh
pio run -e external-interrupt-and-timer0
```

It generated `firmware.elf` and `firmware.hex` under `.pio/build/external-interrupt-and-timer0/`. This confirms compilation and linking; upload and physical operation remain separate checks.

PlatformIO supplies the AVR compiler, include paths and target-specific project model. Do not substitute desktop GCC, copy Arduino headers into the project or add all exercises to a single CMake executable.

## C, C++ and the explicit entry point

`.c` selects C; `.cpp` selects C++. Direct register operations and AVR interrupt macros work in both languages. The Servo class needs C++, and the Arduino header also contains C++ interfaces, so Arduino checkpoints use `.cpp`. An `.ino` normally gets converted into C++ with extra preprocessing; this repository writes ordinary source files directly and includes its own headers.

Bare AVR environments omit `framework = arduino` and provide their own `main()` and hardware initialization. Arduino environments link the framework but also provide their own `main()`:

```cpp
#include <Arduino.h>

int main(void) {
    init();
    // Configure the exercise's registers and libraries here.
    while (1) {
        // Exercise code.
    }
}
```

The Arduino AVR core's default `main()` is in the framework archive. The application's `main()` satisfies the entry-point reference, so the default entry object is not pulled in. The firmware must not also compile that default `main.cpp` as a separate application source. There is no need to define dummy `setup()` or `loop()` functions. Validate the actual link when changing frameworks or build systems.

On the Mega, `init()` initializes the Arduino time base, ADC and other runtime settings and enables interrupts. Examples configuring interrupt registers call `cli()` after `init()` and `sei()` once configuration is complete. The Mega's default `initVariant()` is empty; these examples do not reproduce USB initialization for boards with native USB or the default `serialEventRun()` dispatch. They target this board specifically.

The serial checkpoints call `Serial.begin(9600)` and keep interrupts enabled for buffered serial output. `delay()` also relies on the Arduino time base.

## Timer ownership

The Arduino AVR core uses Timer0 for `millis()`, `micros()` and `delay()`. The two custom Timer0 overflow examples are bare AVR builds: they neither call `init()` nor link Arduino. Combining their ISR with Arduino's Timer0 ISR would create a vector conflict, and reconfiguring that timer would break the framework's timing assumptions.

The Servo library uses Timer5 first on the Mega for this one-servo example; D9 is the signal output, not a requirement to use Timer2 hardware PWM. Do not add your own Timer5 ISR or reconfigure that timer while Servo owns it.

The overflow examples retain the classroom approach: prescaler 64, preload 6, and 1,000 overflows per nominal second. At 16 MHz each 250-tick interval is nominally 1 ms. ISR entry/reload latency and oscillator tolerance make the physical period approximate; a precision clock would need a different timing design or compensation. The combined example toggles PA0 once per nominal second, so its complete on/off cycle is approximately two seconds.

## Upload and serial monitor

The configured board is an **Arduino Mega 2560 with its normal bootloader**, not merely a standalone ATmega2560 chip. A standalone chip needs a programmer and appropriate clock/fuse settings. `F_CPU` describes the clock to the compiler; it does not set the oscillator or program fuses.

```sh
pio device list
pio run -e adc-threshold
pio run -e adc-threshold -t upload
pio device monitor -b 9600
```

If automatic port detection fails, use the port listed on your machine:

```sh
pio run -e adc-threshold -t upload --upload-port /dev/ttyACM0
pio device monitor --port /dev/ttyACM0 -b 9600
```

`/dev/ttyACM0` is only an example; clones may use `/dev/ttyUSB0`. Close any other serial monitor before uploading. On Linux, follow PlatformIO's udev/serial permission instructions for your distribution if access is denied. Do not run the IDE as root to work around it.

For the moving-average experiment, edit `ADC_WINDOW_SIZE` in its environment in `platformio.ini`, for example 5, 10 or 20. The accepted range is 1..64. The sample interval is 100 ms; a larger window smooths more and responds more slowly.

## Debugging and MPLAB

The historical MPLAB configuration uses XC8 3.10, ATmega DFP 3.6.299 and the Simulator. It is preserved at `legacy/mplab/original-project.X/`. Open that project in MPLAB X if you need to revisit the original simulator configuration. Its registered source is `pratica-05.c`; it is not the new build entry point. Local absolute tool paths may need regeneration on another computer.

The MPLAB Makefile delegates to `nbproject/` files. `project.xml` and `configurations.xml` describe the project; generated makefiles implement its builds. Private configuration and local makefiles contain machine-specific information. `build/`, `dist/`, ELF/HEX/MAP files and logs are outputs, not source material. Old outputs remain locally ignored and are not validation of the new examples.

The Mega does not have an onboard debug probe. CLion's Run/Debug buttons do not make USB bootloader upload equivalent to hardware debugging. PlatformIO documents `avr-stub` and `simavr` options for this board, with extra configuration and limitations. Keep MPLAB simulation available for the classroom workflow; hardware breakpoints require a supported, deliberately configured debugging setup.

## Ignored local files

The current `.gitignore` excludes `.idea/`, `.pio/`, `build/`, `dist/`, `debug/`, `.generated_files/`, `cmake-build-*/`, `compile_commands.json`, firmware/object files and logs. It also excludes MPLAB private/local settings, `.venv/` and `.env`. Sources, `platformio.ini`, course PDFs and project documentation remain eligible for version control.

These rules were verified with `git check-ignore` using temporary Git metadata because this workspace does not yet have an initialized repository. After initializing Git locally, confirm the rules with:

```sh
git check-ignore -v .idea/workspace.xml .pio/build/external-interrupt-and-timer0/firmware.elf .pio/build/external-interrupt-and-timer0/firmware.hex
git status --short --ignored
```

Ignore rules apply to untracked files; they do not remove files already committed to an existing repository.

## Git and publishing

Run these in the project root on your own machine after reviewing the files:

```sh
git init -b main
git status --short --ignored
git add README.md .gitignore .editorconfig platformio.ini docs labs legacy scripts
git diff --cached --stat
git commit -m "Organize ATmega2560 study checkpoints"
```

If Git asks for your identity, set your own name and email before committing. The `.git` directory in the agent workspace was mounted read-only, so Git initialization could not be completed there; it is an environment limitation, not a firmware problem. If the restriction persists in CLion's terminal, copy the organized project to a normal writable folder, excluding workspace infrastructure, old build output and the empty protected `.git` directory, then initialize it there.

Create an empty GitHub repository, then use the URL GitHub gives you:

```sh
git remote add origin <your-repository-url>
git push -u origin main
```

Git initialization and a local commit do not publish anything.

## Future boards

The transferable parts are explicit source layout, dependencies, repeatable builds and hardware notes. Register names and initialization remain target-specific. An ESP32 project may use Arduino or ESP-IDF; ESP-IDF has its own entry-point convention. A Raspberry Pi running Linux uses a normal Linux application/cross-compilation setup; a Raspberry Pi Pico is a microcontroller and uses a different SDK. Choose the future setup once the exact board and runtime are known.

## References

- [CLion PlatformIO integration](https://www.jetbrains.com/help/clion/platformio.html)
- [PlatformIO installation](https://docs.platformio.org/en/latest/core/installation/index.html)
- [Mega 2560 configuration and debugging](https://docs.platformio.org/en/latest/boards/atmelavr/megaatmega2560.html)
- [Selecting source files](https://docs.platformio.org/en/latest/projectconf/sections/env/options/build/build_src_filter.html)
- [Arduino sketch build process](https://docs.arduino.cc/arduino-cli/sketch-build-process)
- [Arduino AVR entry point](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/main.cpp)
- [Arduino AVR initialization and Timer0](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/wiring.c)
- [Mega port/pin mapping](https://github.com/arduino/ArduinoCore-avr/blob/master/variants/mega/pins_arduino.h)
- [Servo AVR timer selection](https://github.com/arduino-libraries/Servo/blob/1.2.2/src/avr/ServoTimers.h)
