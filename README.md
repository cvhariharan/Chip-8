# Chip-8 Emulator
This is a [Chip-8](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) emulator written in C. Currently a random maze generator rom is hardcoded for testing purposes. An assembler is also in the works. SDL is required to compile the code.  
  
Can be compiled using gcc as follows
```
gcc chip8.c -o chip8 `sdl2-config --cflags --libs`
```

![Random maze generator](maze.png?raw=true)