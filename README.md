# clib

## Dependencies

Requires "make"

## Setup

1. Run setup.sh to setup the auto-compiler
2. Run build.sh to compile your program(s)

## How to use

Add program files to src/main, and supporting files to src/std

clib standard files such as string.h are included in "src/std"

## Customising Compilation

Edit .config/cfg/config.cfg to change

- Where to look for main files (project entry points)
- Where to look for component files (not project entry points)
- Where program files are output
- Where object files are output
- Where to save the Makefile
- Make instructions

If you have compiliation errors running setup.sh, see .config/scripts/Makefile
