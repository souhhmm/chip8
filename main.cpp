// clang-format off
#include <iostream>
#include "chip8.h"
// clang-format on

chip8 emu;

int main(int argc, char **argv) {
    emu.initialize();
    if (emu.load_rom("roms/2-ibm-logo.8o")) {
        std::cout << "success" << "\n";
    }

    // for (int i = 0; i < 50; i++) {
    //     std::cout << (int)emu.memory[0x200 + i] << "\n";
    // }
}