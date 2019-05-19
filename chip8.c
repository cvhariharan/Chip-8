#define RECT_SIZE 8
#define WINDOW_W 512
#define WINDOW_H 256
#define LITTLE_ENDIAN 0
#define FRAME_DELAY 17

#include<stdio.h>
#include<stdlib.h> 
#include<time.h>
#include<SDL2/SDL.h>


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

SDL_Renderer* renderer;

byte get_low_byte(word);
byte get_high_byte(word);
void createGraphics(SDL_Window*, int [][8], int, int, int);
void draw(int, int, int, SDL_Window*);

int mem_init(int);
int reg_init(int);
void cpu_init();

int keycodes[16] = {
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_4,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_R,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_F,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_X,
    SDL_SCANCODE_C,
    SDL_SCANCODE_V
};

byte fonts[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

int main() {
    int quit = 0;
    // Instruction register
    word ir;
    int running = 1;
    pc = 0x200;
    mem_init(0xFFF);
    reg_init(0x0F);
    cpu_init();

    SDL_Event event;

    SDL_Window* window = NULL;
    // SDL_Surface* surface = NULL;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Could not init SDL!\n");
    }
    else {
        window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
        // draw(80, 0, 0, window);
        memory[512] = 0xe0;
memory[513] = 0x00;
memory[514] = 0x2a;
memory[515] = 0xa2;
memory[516] = 0x0c;
memory[517] = 0x60;
memory[518] = 0x08;
memory[519] = 0x61;
memory[520] = 0x1f;
memory[521] = 0xd0;
memory[522] = 0x09;
memory[523] = 0x70;
memory[524] = 0x39;
memory[525] = 0xa2;
memory[526] = 0x1f;
memory[527] = 0xd0;
memory[528] = 0x48;
memory[529] = 0xa2;
memory[530] = 0x08;
memory[531] = 0x70;
memory[532] = 0x1f;
memory[533] = 0xd0;
memory[534] = 0x04;
memory[535] = 0x70;
memory[536] = 0x57;
memory[537] = 0xa2;
memory[538] = 0x1f;
memory[539] = 0xd0;
memory[540] = 0x08;
memory[541] = 0x70;
memory[542] = 0x66;
memory[543] = 0xa2;
memory[544] = 0x1f;
memory[545] = 0xd0;
memory[546] = 0x08;
memory[547] = 0x70;
memory[548] = 0x75;
memory[549] = 0xa2;
memory[550] = 0x1f;
memory[551] = 0xd0;
memory[552] = 0x28;
memory[553] = 0x12;
memory[554] = 0x00;
memory[555] = 0xff;
memory[556] = 0x00;
memory[557] = 0xff;
memory[558] = 0x00;
memory[559] = 0x3c;
memory[560] = 0x00;
memory[561] = 0x3c;
memory[562] = 0x00;
memory[563] = 0x3c;
memory[564] = 0x00;
memory[565] = 0x3c;
memory[566] = 0x00;
memory[567] = 0xff;
memory[568] = 0xff;
memory[569] = 0xff;
memory[570] = 0xff;
memory[571] = 0x00;
memory[572] = 0x38;
memory[573] = 0x00;
memory[574] = 0x3f;
memory[575] = 0x00;
memory[576] = 0x3f;
memory[577] = 0x00;
memory[578] = 0x38;
memory[579] = 0x00;
memory[580] = 0xff;
memory[581] = 0x00;
memory[582] = 0xff;
memory[583] = 0x00;
memory[584] = 0x00;
memory[585] = 0x80;
memory[586] = 0x00;
memory[587] = 0xe0;
memory[588] = 0x00;
memory[589] = 0xe0;
memory[590] = 0x00;
memory[591] = 0x80;
memory[592] = 0x00;
memory[593] = 0x80;
memory[594] = 0x00;
memory[595] = 0xe0;
memory[596] = 0x00;
memory[597] = 0xe0;
memory[598] = 0xf8;
memory[599] = 0x80;
memory[600] = 0xfc;
memory[601] = 0x00;
memory[602] = 0x3e;
memory[603] = 0x00;
memory[604] = 0x3f;
memory[605] = 0x00;
memory[606] = 0x3b;
memory[607] = 0x00;
memory[608] = 0x39;
memory[609] = 0x00;
memory[610] = 0xf8;
memory[611] = 0x00;
memory[612] = 0xf8;
memory[613] = 0x00;
memory[614] = 0x00;
memory[615] = 0x03;
memory[616] = 0x00;
memory[617] = 0x07;
memory[618] = 0x00;
memory[619] = 0x0f;
memory[620] = 0x00;
memory[621] = 0xbf;
memory[622] = 0x00;
memory[623] = 0xfb;
memory[624] = 0x00;
memory[625] = 0xf3;
memory[626] = 0x00;
memory[627] = 0xe3;
memory[628] = 0xe0;
memory[629] = 0x43;
memory[630] = 0xe0;
memory[631] = 0x00;
memory[632] = 0x80;
memory[633] = 0x00;
memory[634] = 0x80;
memory[635] = 0x00;
memory[636] = 0x80;
memory[637] = 0x00;
memory[638] = 0x80;
memory[639] = 0x00;
memory[640] = 0xe0;
memory[641] = 0x00;
memory[642] = 0xe0;
memory[643] = 0x00;
        while(!quit) {
            byte low = memory[pc++];
            byte high = memory[pc];
            pc++;
            ir = (high << 8) + low;
            printf("IR: %x\n", ir);
            // if(ir == 0x0000) {
            //     quit = 1;
            // }

            // To exit the emulator
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = 1;
                }
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
                                SDL_RenderClear(renderer);
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
                                pc += 4;
                            }
                            break;
                case 0x4000:
                            if(v[x] != kk) {
                                // Skip next instruction
                                pc += 4;
                            }
                            break;
                case 0x5000:
                            if(v[x] == v[y]) {
                                // Skip next instruction
                                pc += 4;
                            }
                            break;
                case 0x6000:
                            // Set vx = kk
                            v[x] = kk;
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
                                pc += 4;
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
                            draw(n, v[x], v[y], window);
                            pc += 2;
                            break;
                case 0xE000:
                            if(kk == 0x9E) {
                                // wait for keyboard and skip if vx key is pressed
                                int q = 0;
                                while(!q) {
                                    while(SDL_PollEvent(&event)){
                                        switch(event.type) {
                                            case SDL_KEYDOWN:
                                                            if(event.key.keysym.scancode == keycodes[v[x]]) {
                                                                // skip next instruction
                                                                pc += 4;
                                                            }
                                                            q = 1;
                                                            break;
                                        }
                                    }
                                }
                            }
                            else if(kk == 0xA1) {
                                // Skip if vx is not pressed
                                int q = 0;
                                while(!q) {
                                    while(SDL_PollEvent(&event)){
                                        switch(event.type) {
                                            case SDL_KEYDOWN:
                                                            if(event.key.keysym.scancode != keycodes[v[x]]) {
                                                                // skip next instruction
                                                                pc += 4;
                                                            }
                                                            q = 1;
                                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                case 0xF000:
                            if(kk == 0x07) {
                                v[x] = delay_reg;
                            }
                            else if(kk == 0x0A) {
                                // Wait for key press and store in vx
                                int q = 0;
                                while(!q) {
                                    while(SDL_PollEvent(&event)){
                                        switch(event.type) {
                                            case SDL_KEYDOWN:
                                                            v[x] = event.key.keysym.scancode;
                                                            q = 1;
                                                            break;
                                        }
                                    }
                                }
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
                                I = v[x];
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
                                    memory[I+i] = v[i];
                                }
                            }
                            else if(kk == 0x65) {
                                // read into v0 to vx from memory I and onwards
                                for(int i = 0; i < x; i++) {
                                    v[i] = memory[I+i];
                                }
                            }
                            pc += 2;
                            break;
            }
            SDL_Delay(FRAME_DELAY);
        }
    }
    for(int i = 0; i < 16; i++) {
        printf("%x\n", v[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free(v);
    free(memory);
    return 0;
}

// Initializes the memory and registers

int mem_init(int size) {
    memory = (byte*)malloc(sizeof(byte)*size);
    for(int i = 0; i < 0xFFF; i++) {
        memory[i] = 0;
    }
    memcpy(memory, fonts, sizeof(fonts));
    return 1;
}

int reg_init(int size) {
    v = (byte*)malloc(sizeof(byte)*size);
    for(int i = 0; i < 0x0F; i++) {
        v[i] = 0;
    }
    return 1;
}

void cpu_init() {
    sp = -1;
    pc = 0x200;
    I = 0;
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

void draw(int n, int x, int y, SDL_Window* window) {
    int g[n][8];
    for(int i = I; i < I + n; i++) {
        int mask = 0x80;
        for(int j = 0; j < 8; j++) {
            printf("Mem - %x\n", memory[i]);
            g[i-I][j] = (memory[i] & mask) >> (8-j-1);
            mask = mask >> 1;
        }
    }
    createGraphics(window, g, n, x, y);
}

void createGraphics(SDL_Window* window, int g[][8], int h, int x, int y) {
    SDL_Event event;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    int quit = 0;
    while(!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_RenderClear(renderer);
        for(int i = 0; i < h; i++) {
            for(int j = 0; j < 8; j++) {
                // printf("%d ", g[i][j]);
                if(g[i][j]) {
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
                }
                SDL_Rect rect = {(x + j*RECT_SIZE) % WINDOW_W, (y + i*RECT_SIZE) % WINDOW_H, RECT_SIZE, RECT_SIZE};
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(17);
    }
}