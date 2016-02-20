#include <stdint.h>
#include "sysio.h"

#define MASTER_PIC_COMMAND 0x20
#define MASTER_PIC_DATA    0x21
#define SLAVE_PIC_COMMAND  0xA0
#define SLAVE_PIC_DATA     0xA1

void init_pic() {
    uint8_t a, b;
    a = inb(MASTER_PIC_DATA);
    b = inb(SLAVE_PIC_DATA);

    /* Remap IRQ offsets on the PIC (Programmable Interrupt Controller) */
    outb(MASTER_PIC_COMMAND, 0x10);
    io_wait();
    outb(SLAVE_PIC_COMMAND, 0x10);
    io_wait();
    outb(MASTER_PIC_DATA, 0x20);
    io_wait();
    outb(SLAVE_PIC_DATA, 0x28);
    io_wait();
    outb(MASTER_PIC_DATA, 0x04);
    io_wait();
    outb(SLAVE_PIC_DATA, 0x02);
    io_wait();
    outb(MASTER_PIC_DATA, 0x01);
    io_wait();
    outb(SLAVE_PIC_DATA, 0x01);
    io_wait();
    outb(MASTER_PIC_DATA, 0x0);
    io_wait();
    outb(SLAVE_PIC_DATA, 0x0);
    io_wait();

    outb(MASTER_PIC_DATA, a);
    outb(SLAVE_PIC_DATA, b);
}
