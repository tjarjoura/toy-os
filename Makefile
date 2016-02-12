CFLAGS:=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

LIBS:=-lgcc
LDFLAGS:=-nostdlib

CC:=i686-elf-gcc
AS:=i686-elf-as

INCLUDEDIR:=include

all: iso

include drivers/make.config
include kernel/make.config
include lib/make.config

OBJS:= $(DRIVER_OBJS) $(KERNEL_OBJS) $(LIB_OBJS)

myos.bin: $(OBJS)
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS)

%.o: %.s
	$(AS) $< -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -I include

iso: myos.bin
	cp myos.bin isodir/boot/myos.bin
	grub-mkrescue -d /usr/lib/grub/i386-pc/ -o myos.iso isodir

clean:
	rm $(OBJS) myos.bin myos.iso

