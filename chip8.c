#include<stdio.h>
#include <stdlib.h> 
#include<time.h> 

#define LITTLE_ENDIAN 0

typedef unsigned short int word;
typedef unsigned char byte;

byte* memory;
byte* v; // General-purpose registers

// Special purpose registers for timer and sound
byte delay_reg;
byte sound_reg;

word pc;
word I; // Index register
byte sp;
unsigned short int stack[16];

byte get_low_byte(word);
byte get_high_byte(word);

int mem_init(int);
int reg_init(int);

int main() {
    // Instruction register
    word ir;
    int running = 1;
    pc = 0x200;
    mem_init(0xFFF);
    reg_init(0x0F);

    while(1) {
        byte high = memory[pc++];
        byte low = memory[pc];
        ir = (high << 8) + low;
        if(ir == 0x00FD) {
            break;
        }
        word nnn = ir & 0x0FFF;
        byte n = ir & 0x000F;
        byte x = get_high_byte(ir) & 0x0F;
        byte y = (get_low_byte(ir) & 0xF0) >> 4;
        byte kk = get_low_byte(ir);

        switch(ir & 0xF000) {
            case 0x0000:
                        if(ir == 0x00E0) {
                            // clear display
                        }
                        else if(ir == 0x00EE) {
                            // Return
                            pc = stack[sp--];
                        }
                        break;
            case 0x1000:
                        // Jump to address. Cannot return
                        pc = nnn;
                        break;
            case 0x2000:
                        // Call subroutine, can return
                        stack[++sp] = pc;
                        pc = nnn;
                        break;
            case 0x3000:
                        if(v[x] == kk) {
                            // Skip next instruction
                            pc += 2;
                        }
                        break;
            case 0x4000:
                        if(v[x] != kk) {
                            // Skip next instruction
                            pc += 2;
                        }
                        break;
            case 0x5000:
                        if(v[x] == v[y]) {
                            // Skip next instruction
                            pc += 2;
                        }
                        break;
            case 0x6000:
                        // Set vx = kk
                        v[x] == kk;
                        pc += 2;
                        break;
            case 0x7000:
                        v[x] = v[x] + kk;
                        pc += 2;
                        break;
            case 0x8000:
                        if(n == 0x0) {
                            v[x] = v[y];
                        }
                        else if(n == 0x1) {
                            v[x] = v[x] | v[y];
                        }
                        else if(n == 0x2) {
                            v[x] = v[x] & v[y];
                        }
                        else if(n == 0x3) {
                            v[x] = v[x] ^ v[y];
                        }
                        else if(n == 0x4) {
                            word res = v[x] + v[y];
                            v[0x0F] = res >> 8;
                        }
                        else if(n == 0x5) {
                            if(v[x] > v[y]) {
                                v[0x0F] = 1;
                            }
                            else {
                                v[0x0F] = 0;
                            }
                            v[x] = v[y] - v[x];
                        }
                        else if(n == 0x6) {
                            // Shift right and store LSB of vx
                            v[0x0F] = v[x] & 0x01;
                            v[x] = v[x] >> 1;
                        }
                        else if(n == 0x7) {
                            if(v[y] > v[x]) {
                                v[0x0F] = 1;
                            }
                            else {
                                v[0x0F] = 0;
                            }
                            v[x] = v[y] - v[x];
                        }
                        else if(n == 0xE) {
                            // Shift left and store MSB of vx
                            v[0x0F] = v[x] & 0x80;
                            v[x] = v[x] << 1;
                        }
                        pc += 2;
                        break;
            case 0x9000:
                        if(v[x] != v[y]) {
                            // Skip next instruction
                            pc += 2;
                        }
                        break;
            case 0xA000:
                        I = nnn;
                        pc += 2;
                        break;
            case 0xB000:
                        pc = nnn + v[0];
                        break;
            case 0xC000:
                        srand(time(0));
                        int r = rand() % 256;
                        v[x] = r & kk;
                        pc += 2;
                        break;
            case 0xD000:
                        // Draw sprite
                        break;
            case 0xE000:
                        if(kk == 0x9E) {
                            // Check keyboard and skip if vx key is pressed
                        }
                        else if(kk == 0xA1) {
                            // Skip if vx is not pressed
                        }
                        break;
            case 0xF000:
                        if(kk == 0x07) {
                            v[x] = delay_reg;
                        }
                        else if(kk == 0x0A) {
                            // Wait for key press and store in vx
                        }
                        else if(kk == 0x15) {
                            delay_reg = v[x];
                        }
                        else if(kk == 0x18) {
                            sound_reg = v[x];
                        }
                        else if(kk == 0x1E) {
                            I += v[x];
                        }
                        else if(kk == 0x29) {
                            // location of sprite
                        }
                        else if(kk == 0x33) {
                            // store bcd representation of vx in I, I+1 and I+2 memory locs
                            int num = v[x];
                            for(int i = 2; i >= 0; i++) {
                                int d = num % 10;
                                num = num / 10;
                                memory[I + i] = d; 
                            }
                        }
                        else if(kk == 0x55) {
                            // store v0 to vx from memory I and onwards
                            for(int i = 0; i < x; i++) {
                                memory[I] = v[i];
                                I += 1;
                            }
                        }
                        else if(kk == 0x65) {
                            // read into v0 to vx from memory I and onwards
                            for(int i = 0; i < x; i++) {
                                v[i] = memory[I];
                                I += 1;
                            }
                        }
                        pc += 2;
                        break;
        }
    }

    free(v);
    free(memory);
    return 0;
}

// Initializes the memory and registers

int mem_init(int size) {
    memory = (byte*)malloc(sizeof(byte)*size);
    return 1;
}

int reg_init(int size) {
    v = (byte*)malloc(sizeof(byte)*size);
    return 1;
}

// Returns the lower or higher order bits
// according to the endianess

byte get_low_byte(word w) {
    unsigned short int n = 0x00FF;
    if(LITTLE_ENDIAN) 
        return w >> 8;
    else
        return w & n;
}

byte get_high_byte(word w) {
    unsigned short int n = 0x00FF;
    if(LITTLE_ENDIAN) 
        return w & n;
    else
        return w >> 8;
}