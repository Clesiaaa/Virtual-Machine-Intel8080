gcc -Wall -I./include -c src/core/cpu.c -o obj/cpu.o

gcc -Wall -I./include -c src/core/ram.c -o obj/ram.o

gcc -Wall -I./include -c src/core/alu.c -o obj/alu.o

gcc -Wall -I./include -c src/execution_engine/instructions.c -o obj/instructions.o

gcc -Wall -I./include -c src/compiler/compile.c -o obj/compile.o

gcc -Wall -I./include -c src/fs/filesystem.c -o obj/filesystem.o

gcc -Wall -I./include -c main.c -o obj/main.o

gcc obj/main.o obj/cpu.o obj/ram.o obj/instructions.o obj/alu.o obj/compile.o obj/filesystem.o -o emulator
