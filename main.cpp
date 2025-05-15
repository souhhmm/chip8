#include <iostream>
#include "chip8.h"

chip8 emu;

int main(int argc, char **argv) {
    emu.initialize();
    emu.load_rom("roms/ibm-logo.ch8");

    for (int i=0; i<100; i++) {
        emu.emulate_cycle();
    }
    
    emu.terminal_render();
}