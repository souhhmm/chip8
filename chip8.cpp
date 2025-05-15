// clang-format off
#include "chip8.h"
#include <string.h>
#include <iostream>

typedef unsigned char byte;   // 0-255
typedef unsigned short word;  // 0-65535

byte fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

// clang-format on

chip8::chip8() {};
chip8::~chip8() {};

void chip8::initialize() {
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    memset(registers, 0, sizeof(registers));
    memset(stack, 0, sizeof(stack));
    memset(memory, 0, sizeof(memory));

    for (int i = 0; i < 80; i++) {
        memory[i] = fontset[i];
    }

    delay_timer = 0;
    sound_timer = 0;
}

bool chip8::load_rom(const char* filename) {
    FILE* rom = fopen(filename, "rb");

    if (rom == NULL) {
        fputs("file error", stderr);
        return false;
    }

    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    rewind(rom);

    // TODO: memory error

    // rom is loaded into 0x200 and onwards as 0x000-0x1FF is for the interpreter
    fread(&memory[0x200], 1, rom_size, rom);
    fclose(rom);

    return true;
}

void chip8::emulate_cycle() {
    opcode = memory[pc] << 8 | memory[pc + 1];
    switch (opcode & 0xF000) {
        default:
            std::cout << "unknown opcode: " << opcode << "\n";
    }
}
