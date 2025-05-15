#include <iostream>
#include "chip8.h"

chip8 emu;

int main(int argc, char **argv) {
    emu.initialize();
    emu.load_rom("roms/2-ibm-logo.8o");
}