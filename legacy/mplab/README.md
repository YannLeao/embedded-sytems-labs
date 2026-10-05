# Original MPLAB project

`original-project.X/` preserves the four original source files and MPLAB project files from before the repository reorganization. Source syntax, Portuguese identifiers, filename numbering and the servo example's missing semicolon are intentionally retained here as historical evidence. Use the corrected active examples in `labs/` for new builds.

The original configuration selects `pratica-05.c`, targets ATmega2560 with XC8 3.10 and uses the Simulator. `main.c` contains the combined interrupt example, `pratica-06.c` the potentiometer example, and `pratica-07.c` the servo example. Generated makefiles explicitly compile registered sources as C (`-x c`); simply renaming an Arduino source is not enough to make this historical XC8 build a C++/Arduino project.

MPLAB private settings and local compiler paths were preserved locally but are ignored by Git. Reopening the project on another machine may regenerate these files. Generated build output and logs stayed outside this archive and are ignored. Do not use old ELF files to judge the active examples.
