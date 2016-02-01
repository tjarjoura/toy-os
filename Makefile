CFLAGS:=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

OBJS:=boot.o dt.o descriptor_table.o kernel.o vgatext.o
LIBS:=-lgcc
LDFLAGS:=-nostdlib

CC:=i686-elf-gcc
AS:=i686-elf-as

all: iso

myos.bin: $(OBJS)
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS)

%.o: %.s
	$(AS) $< -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

iso: myos.bin
	cp myos.bin isodir/boot/myos.bin
	grub-mkrescue -d /usr/lib/grub/i386-pc/ -o myos.iso isodir

clean:
	rm *.o myos.bin myos.iso

