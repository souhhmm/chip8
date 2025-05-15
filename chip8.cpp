#include "chip8.h"
#include <string.h>
#include <iostream>
#include <time.h>

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
    memset(display, 0, sizeof(display));

    // load the fontset
    for (int i = 0; i < 80; i++) {
        memory[i + 0x050] = fontset[i];
    }

    draw_flag = true;

    delay_timer = 0;
    sound_timer = 0;

    srand(time(NULL));
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

    std::cout << "file size: " << rom_size << "\n";

    // rom is loaded into 0x200 and onwards as 0x000-0x1FF is for the interpreter
    fread(&memory[0x200], 1, rom_size, rom);
    fclose(rom);

    return true;
}

void chip8::emulate_cycle() {
    // fetch
    opcode = memory[pc] << 8 | memory[pc + 1];

    // decode
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F) {
                case 0x0000:  // 0x00E0: clears the screen
                    for (int i = 0; i < 2048; ++i) display[i] = 0x0;
                    draw_flag = true;
                    pc += 2;
                    break;

                case 0x000E:  // 0x00EE: returns from subroutine
                    --sp;
                    pc = stack[sp];
                    pc += 2;
                    break;

                default:
                    std::cout << "unknown opcode\n";
            }
            break;

        case 0x1000:  // 0x1NNN: jumps to address NNN
            pc = opcode & 0x0FFF;
            break;

        case 0x2000:
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
            break;

        case 0x6000:  // 0x6XNN: sets VX to NN
            registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;

        case 0x7000:  // 0x7XNN: adds NN to VX
            registers[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
            break;

        case 0xA000:  // ANNN: sets I to the address NNN
            I = opcode & 0x0FFF;
            pc += 2;
            break;

        case 0xD000: {  // DXYN: draw at coordinate x, y with height N
            word x = registers[(opcode & 0x0F00) >> 8];
            word y = registers[(opcode & 0x00F0) >> 4];
            word height = opcode & 0x000F;
            word pixel;

            registers[0xF] = 0;

            for (int j = 0; j < height; j++) {
                pixel = memory[I + j];
                for (int i = 0; i < 8; i++) {
                    if ((pixel & (0x80 >> i)) != 0) {
                        int curx = (x + i);
                        int cury = (y + j);
                        int pos = curx + (cury * 64);

                        if (display[pos] == 1) {
                            registers[0xF] = 1;
                        }
                        display[pos] ^= 1;
                    }
                }
            }

            draw_flag = true;
            pc += 2;
        } break;

        default:
            std::cout << "unknown opcode: " << opcode << "\n";
    }

    if (delay_timer > 0) {
        --delay_timer;
    }

    if (sound_timer > 0) {
        if (sound_timer == 1) {
            std::cout << "beep" << "\n";
        }
        --sound_timer;
    }
}

void chip8::terminal_render() {
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            if (display[(y * 64) + x] == 1)
                std::cout << "█";
            else
                std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}