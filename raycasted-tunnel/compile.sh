#cc -c -o buildobjs/inner_lighted_tile.o inner_lighted_tile.s
clang -c -g -Wall -O2 -o buildobjs/chrono.o chrono.c
clang -c -g -Wall -O2 -o buildobjs/tunnel.o tunnel.c
clang -c -g -Wall -O2 -o buildobjs/inner_specialization.o inner_specialization.c

cd buildobjs
clang -o ../tunnel.elf tunnel.o chrono.o inner_specialization.o -lSDL -lm
#clang -o ../tunnel.elf tunnel.o chrono.o inner_lighted_tile.o -lSDL -lm
