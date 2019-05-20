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

byte memory[0xFFF];
byte v[0x0F]; // General-purpose registers
int graph_matrix[32][64];

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
void createGraphics(SDL_Renderer*, int, int, int);
void draw(int, int, int, SDL_Renderer*);

int mem_init(int);
int reg_init(int);
void cpu_init();

int keycodes[16] = {
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_X,
    SDL_SCANCODE_C,
    SDL_SCANCODE_4,
    SDL_SCANCODE_R,
    SDL_SCANCODE_F,
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
    // mem_init(0xFFF);
    // reg_init(0x0F);
    cpu_init();

    SDL_Event event;

    SDL_Window* window = NULL;
    // SDL_Surface* surface = NULL;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Could not init SDL!\n");
    }
    else {
        window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        // draw(80, 0, 0, window);
     memory[512] = 0x00;
memory[513] = 0x60;
memory[514] = 0x00;
memory[515] = 0x61;
memory[516] = 0x22;
memory[517] = 0xa2;
memory[518] = 0x01;
memory[519] = 0xc2;
memory[520] = 0x01;
memory[521] = 0x32;
memory[522] = 0x1e;
memory[523] = 0xa2;
memory[524] = 0x14;
memory[525] = 0xd0;
memory[526] = 0x04;
memory[527] = 0x70;
memory[528] = 0x40;
memory[529] = 0x30;
memory[530] = 0x04;
memory[531] = 0x12;
memory[532] = 0x00;
memory[533] = 0x60;
memory[534] = 0x04;
memory[535] = 0x71;
memory[536] = 0x20;
memory[537] = 0x31;
memory[538] = 0x04;
memory[539] = 0x12;
memory[540] = 0x1c;
memory[541] = 0x12;
memory[542] = 0x40;
memory[543] = 0x80;
memory[544] = 0x10;
memory[545] = 0x20;
memory[546] = 0x40;
memory[547] = 0x20;
memory[548] = 0x10;
memory[549] = 0x80;

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
            byte x = (ir & 0x0F00) >> 8;
            byte y = (ir & 0x00F0) >> 4;
            byte kk = ir & 0x00FF;

            printf("nnn - %x, n - %x, x - %x, y - %x, kk - %x\n", nnn, n, x, y, kk);

            switch(ir & 0xF000) {
                case 0x0000:
                            if(ir == 0x00E0) {
                                // clear display
                                printf("Clear display\n");
                                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
                                SDL_RenderFillRect(renderer, NULL);
                                SDL_RenderPresent(renderer); 
                            }
                            else if(ir == 0x00EE) {
                                // Return
                                printf("Return\n");
                                pc = stack[sp--];
                            }
                            break;
                case 0x1000:
                            // Jump to address. Cannot return
                            printf("Jump\n");
                            pc = nnn;
                            break;
                case 0x2000:
                            // Call subroutine, can return
                            printf("Call\n");
                            stack[++sp] = pc;
                            pc = nnn;
                            break;
                case 0x3000:
                            if(v[x] == kk) {
                                // Skip next instruction
                                printf("Skip vx = kk\n");
                                pc += 2;
                            }
                            break;
                case 0x4000:
                            if(v[x] != kk) {
                                // Skip next instruction
                                printf("Skip vx != kk\n");
                                pc += 2;
                            }
                            break;
                case 0x5000:
                            if(v[x] == v[y]) {
                                // Skip next instruction
                                printf("Skip vx = vy\n");
                                pc += 2;
                            }
                            break;
                case 0x6000:
                            // Set vx = kk
                            printf("set vx = kk\n");
                            v[x] = kk;
                            break;
                case 0x7000:
                            printf("Set vx += kk\n");
                            v[x] = v[x] + kk;
                            break;
                case 0x8000:
                            if(n == 0x0) {
                                printf("set vx = vy\n");
                                v[x] = v[y];
                            }
                            else if(n == 0x1) {
                                printf("set vx |= vy\n");
                                v[x] = v[x] | v[y];
                            }
                            else if(n == 0x2) {
                                printf("set vx &= vy\n");
                                v[x] = v[x] & v[y];
                            }
                            else if(n == 0x3) {
                                printf("set vx ^= vy\n");
                                v[x] = v[x] ^ v[y];
                            }
                            else if(n == 0x4) {
                                printf("set vx = vy + vx\n");
                                word res = v[x] + v[y];
                                v[0x0F] = ((res > 255) ? 1 : 0);
                                v[x] = res;
                            }
                            else if(n == 0x5) {
                                printf("set vx -= vy, vx > vy\n");
                                v[0x0F] = (v[x] > v[y]) ? 1 : 0;
                                v[x] = v[y] - v[x];
                            }
                            else if(n == 0x6) {
                                // Shift right and store LSB of vx
                                printf("vx >> 1 LSB\n");
                                v[0x0F] = v[x] & 0x01;
                                v[x] = v[x] >> 1;
                            }
                            else if(n == 0x7) {
                                printf("set vx -= vy vy > vx\n");
                                v[0x0F] = (v[y] > v[x]) ? 1 : 0;
                                v[x] = v[y] - v[x];
                            }
                            else if(n == 0xE) {
                                // Shift left and store MSB of vx
                                printf("vx << 1 MSB\n");
                                v[0x0F] = (v[x] >> 7) & 0x01;
                                v[x] = v[x] << 1;
                            }
                            break;
                case 0x9000:
                            if(v[x] != v[y]) {
                                // Skip next instruction
                                printf("Skip vx != vy\n");
                                pc += 2;
                            }
                            break;
                case 0xA000:
                            printf("I = nnn\n");
                            I = nnn;
                            break;
                case 0xB000:
                            printf("pc = nnn + v0\n");
                            pc = nnn + v[0];
                            break;
                case 0xC000:
                            printf("vx = r & kk\n");
                            srand(time(0));
                            int r = rand() % 256;
                            v[x] = r & kk;
                            break;
                case 0xD000:
                            // Draw sprite
                            // clear display
                            printf("clear and draw\n");
                            // SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
                            // SDL_RenderFillRect(renderer, NULL);
                            // SDL_RenderPresent(renderer); 
                            printf("%d %d\n", v[x], v[y]);
                            draw(n, v[x], v[y], renderer);
                            break;
                case 0xE000:
                            if(kk == 0x9E) {
                                // wait for keyboard and skip if vx key is pressed
                                printf("wait for keypress and skip\n");
                                int q = 0;
                                while(!q) {
                                    while(SDL_PollEvent(&event)){
                                        switch(event.type) {
                                            case SDL_KEYDOWN:
                                                            if(event.key.keysym.scancode == keycodes[v[x]]) {
                                                                // skip next instruction
                                                                pc += 2;
                                                            }
                                                            q = 1;
                                                            break;
                                        }
                                    }
                                }
                            }
                            else if(kk == 0xA1) {
                                // Skip if vx is not pressed
                                printf("wait for not keypress and skip\n");
                                int q = 0;
                                while(!q) {
                                    while(SDL_PollEvent(&event)){
                                        switch(event.type) {
                                            case SDL_KEYDOWN:
                                                            if(event.key.keysym.scancode != keycodes[v[x]]) {
                                                                // skip next instruction
                                                                pc += 2;
                                                            }
                                                            q = 1;
                                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                case 0xF000:
                            // printf("kk - %x\n", kk);
                            if(kk == 0x07) {
                                v[x] = delay_reg;
                            }
                            else if(kk == 0x0A) {
                                // Wait for key press and store in vx
                                printf("wait for keypress and store\n");
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
                                printf("delay = vx\n");
                                delay_reg = v[x];
                            }
                            else if(kk == 0x18) {
                                printf("sound = vx\n");
                                sound_reg = v[x];
                            }
                            else if(kk == 0x1E) {
                                printf("I += vx\n");
                                I += v[x];
                            }
                            else if(kk == 0x29) {
                                // location of sprite
                                printf("I = vx\n");
                                I = v[x];
                            }
                            else if(kk == 0x33) {
                                // store bcd representation of vx in I, I+1 and I+2 memory locs
                                printf("BCD\n");
                                int num = v[x];
                                for(int i = 2; i >= 0; i--) {
                                    int d = num % 10;
                                    num = num / 10;
                                    memory[I + i] = d; 
                                }
                            }
                            else if(kk == 0x55) {
                                printf("reg to mem\n");
                                // store v0 to vx from memory I and onwards
                                for(int i = 0; i < x; i++) {
                                    memory[I+i] = v[i];
                                }
                            }
                            else if(kk == 0x65) {
                                printf("mem to reg\n");
                                // read into v0 to vx from memory I and onwards
                                for(int i = 0; i < x; i++) {
                                    v[i] = memory[I+i];
                                }
                            }
                            break;
            }
            SDL_Delay(100);
        }
    }
    // for(int i = 0; i < 16; i++) {
    //     printf("%x\n", v[i]);
    // }
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // free(v);
    // free(memory);
    return 0;
}

// Initializes the memory and registers

// int mem_init(int size) {
//     memory = (byte*)malloc(sizeof(byte)*size);
//     for(int i = 0; i < 0xFFF; i++) {
//         memory[i] = 0;
//     }
//     memcpy(memory, fonts, sizeof(fonts));
//     return 1;
// }

// int reg_init(int size) {
//     v = (byte*)malloc(sizeof(byte)*size);
//     for(int i = 0; i < size; i++) {
//         v[i] = 0;
//     }
//     return 1;
// }

void cpu_init() {
    sp = -1;
    pc = 0x200;
    I = 0;
}

// Returns the lower or higher order bits
// according to the endianess

// byte get_low_byte(word w) {
//     unsigned short int n = 0x00FF;
//     if(LITTLE_ENDIAN) 
//         return w >> 8;
//     else
//         return w & n;
// }

// byte get_high_byte(word w) {
//     unsigned short int n = 0x00FF;
//     if(LITTLE_ENDIAN) 
//         return w & n;
//     else
//         return w >> 8;
// }

void reset_graph_matrix() {
    for(int i = 0; i < 32; i++) {
        for(int j = 0; j < 64; j++) {
            graph_matrix[i][j] = 0;
        }
    }
}

void draw(int n, int x, int y, SDL_Renderer* renderer) {
    // reset_graph_matrix();
    v[0x0F] = 0;
    for(int i = I; i < I + n; i++) {
        int mask = 0x80;
        for(int j = 0; j < 8; j++) {
            // printf("Mem - %x\n", memory[i]);
            int b = (memory[i] & mask) >> (8-j-1);
            if(graph_matrix[i-I+x][j+y] == 1 && b == 1) {
                v[0x0F] = 1;
            }
            graph_matrix[i-I+x][j+y] = b;
            mask = mask >> 1;
        }
    }
    // for(int i = 0; i < 32; i++) {
    //     for(int j = 0; j < 64; j++) {
    //         printf("%d ", graph_matrix[i][j]);
    //     }
    //     printf("\n");
    // }
    createGraphics(renderer, n, x, y);
}

void createGraphics(SDL_Renderer* renderer, int h, int x, int y) {
    SDL_Event event;
    int quit = 0;
    // while(!quit) {
        // while (SDL_PollEvent(&event)) {
        //     if (event.type == SDL_QUIT) {
        //         quit = 1;
        //     }
        // }
        SDL_RenderClear(renderer);
        for(int i = 0; i < 32; i++) {
            for(int j = 0; j < 64; j++) {
                // printf("%d ", g[i][j]);
                if(graph_matrix[i][j]) {
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
        // SDL_Delay(17);
    // }
}

void probe(char a) {
    switch(a) {
        case 'm':
                for(int i = 0; i < 0xFFF; i++) {
                    printf("%x\n", memory[i]);
                }
                break;
        case 'v':
                for(int i = 0; i < 0xF; i++) {
                    printf("%x\n", v[i]);
                }
                break;
        case 'g':
                for(int i = 0; i < 32; i++) {
                    for(int j = 0; j < 64; j++) {
                        printf("%d ", graph_matrix[i][j]);
                    }
                    printf("\n");
                }
                break;
        case 's':
                for(int i = 0; i < 0xF; i++) {
                    printf("%x\n", stack[i]);
                }
                break;
        case 'o':
                printf("sp - %x\n", sp);
                printf("I - %x\n", I);
                printf("pc - %x\n", pc);
                break;
        default: printf("Next\n");
    }
}