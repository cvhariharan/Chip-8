#include<stdio.h>

#define LITTLE_ENDIAN 1

typedef unsigned short int word;
typedef unsigned char byte;

byte* memory;
byte* v; // General-purpose registers

// Special purpose registers for timer and sound
byte delay_reg;
byte sound_reg;

word pc;
byte sp;

byte get_low_byte(word);
byte get_high_byte(word);

int mem_init(int);
int reg_init(int);

int main() {
    mem_init(0xFFF);
    reg_init(0x0F);
    printf("%02x\n", get_high_byte(0x435A));
    printf("%02x\n", get_low_byte(0x435A));
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