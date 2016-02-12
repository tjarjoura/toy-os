#ifndef SYSIO_H
#define SYSIO_H

/* These are wrappers for the x86 IN and OUT
 * machine instructions for writing to i/o ports
 */

void io_wait();

void outl(int port, uint32_t data);
void outw(int port, uint16_t data);
void outb(int port, uint8_t data);

uint32_t inb(int port);
uint32_t inl(int port);
uint32_t inw(int port);

#endif
