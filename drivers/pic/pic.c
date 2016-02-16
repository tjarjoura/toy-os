#include <stdint.h>

#include "sysio.h"

void init_pic() {
    uint8_t a, b;
    a = inb(0x21);
    b = inb(0xA1);

    /* Remap IRQ offsets on the PIC (Programmable Interrupt Controller) */
    outb(0x20, 0x11);
    io_wait();
    outb(0xA0, 0x11);
    io_wait();
    outb(0x21, 0x20);
    io_wait();
    outb(0xA1, 0x28);
    io_wait();
    outb(0x21, 0x04);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();
    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();
    outb(0x21, 0x0);
    io_wait();
    outb(0xA1, 0x0);
    io_wait();

    outb(0x21, a);
    outb(0xA1, b);
}
