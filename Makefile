FILES = ./build/kernel.asm.o \
				./build/kernel.o \
				./build/idt.asm.o \
				./build/memory.o \
				./build/idt.o \
				./build/io.asm.o\
				./build/heap.o\
				./build/kheap.o\
				./build/paging.o\
				./build/paging.asm.o

INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-ld -T ./src/linker.ld -o ./bin/kernel.bin ./build/kernelfull.o


./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf  ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

./build/idt.asm.o: ./src/idt/idt.asm
	nasm -f elf  ./src/idt/idt.asm -o ./build/idt.asm.o

./build/io.asm.o: ./src/io/io.asm
	nasm -f elf  ./src/io/io.asm -o ./build/io.asm.o

./build/idt.o: ./src/idt/idt.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/idt/idt.c -o ./build/idt.o

./build/memory.o: ./src/memory/memory.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o ./build/memory.o

./build/heap.o: ./src/memory/heap/heap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/heap.o
./build/kheap.o: ./src/memory/heap/kheap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/kheap.o

./build/paging.asm.o: ./src/memory/paging/paging.asm
	nasm -f elf  ./src/memory/paging/paging.asm -o ./build/paging.asm.o

./build/paging.o: ./src/memory/paging/paging.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory/paging/paging.c -o ./build/paging.o

clean:
	rm ./bin/*
	rm ./build/*

run: 
	qemu-system-i386 -hda ./bin/os.bin
