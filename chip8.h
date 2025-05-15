#include <string.h>

#include <iostream>
typedef unsigned char byte;   // 0-255
typedef unsigned short word;  // 0-65535

class chip8 {
   public:
    byte memory[4096];   // 4096 bytes of memory
    byte registers[16];  // 16 8-bit registers from V0 to VF
    word I;              // 16-bit address register I used to access memory
    word pc;             // 16-bit program counter
    word opcode;         // 35 opcodes each 2 bytes long
    word stack[16];      // 16-bit stack
    word sp;             // stack pointer
    byte delay_timer;
    byte sound_timer;
    byte display[64 * 32];
    byte key[16];

    chip8();
    void initialize();
    bool load_rom(const char* filename);
    void emulate_cycle();
    ~chip8();
};