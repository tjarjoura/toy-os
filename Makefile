objs = boot.o kernel.o vgatext.o
cflags = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

all: iso

myos.bin: $(objs)
	i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o vgatext.o -lgcc

boot.o: boot.s
	i686-elf-as boot.s -o boot.o

vgatext.o: vgatext.c
	i686-elf-gcc -c vgatext.c -o vgatext.o $(cflags)

kernel.o: kernel.c
	i686-elf-gcc -c kernel.c -o kernel.o $(cflags)

iso: myos.bin
	cp myos.bin isodir/boot/myos.bin
	grub-mkrescue -d /usr/lib/grub/i386-pc/ -o myos.iso isodir

clean:
	rm *.o myos.bin myos.iso

