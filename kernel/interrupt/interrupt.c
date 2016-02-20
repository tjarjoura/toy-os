#include <stdint.h>
#include "descriptor_table.h"
#include "interrupt.h"
#include "isr.h"
#include "lib.h"
#include "common.h"
#include "sysio.h"

void flush_idt(void *);

static interrupt_handler_t handlers[256];

void set_interrupt_handler(uint8_t int_num, interrupt_handler_t handler) {
    handlers[int_num] = handler;
}

/* Interrupt Descriptor Table */
struct idt_entry {
    uint16_t offset_lo;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t offset_hi;
} __attribute__((packed));

struct idt_entry idt[256];

struct {
    uint16_t size;
    struct idt_entry *idt_addr;
} __attribute__((packed))
idt_r = { .size = sizeof(idt), .idt_addr = idt};

static struct idt_entry idt_entry(uint32_t offset, uint16_t selector, uint8_t flags) {
    struct idt_entry entry;

    entry.offset_lo = 0x0000FFFF & offset;
    entry.offset_hi = (0xFFFF0000 & offset) >> 16;

    entry.selector = selector;
    entry.flags = flags;

    entry.zero = 0x00;

    return entry;
}

void common_interrupt_handler(struct saved_registers saved_registers) {
    kprintf("Received interrupt: %d\n", saved_registers.int_num);
    kprintf("Error code : %d\n", saved_registers.err_code);

    if (handlers[saved_registers.int_num] != NULL)
        handlers[saved_registers.int_num](saved_registers);
}

void common_irq_handler(struct saved_registers saved_registers) {
    kprintf("Received Interrupt(IRQ): %d\n", saved_registers.int_num);

    if (handlers[saved_registers.int_num] != NULL)
        handlers[saved_registers.int_num](saved_registers);

    if (saved_registers.int_num > 40) /* Interrupt came from Slave PIC */
        outb(0xA0, 0x20);
    outb(0x20, 0x20); /* Send EOI signal to PIC controllers */
}

/* These functions are small identical stubs that will take direct the code to
 * our common interrupt handler functions */
void init_idt() {
    idt[0] = idt_entry((uint32_t) isr0, 0x08, 0x8E);
    idt[1] = idt_entry((uint32_t) isr1, 0x08, 0x8E);
    idt[2] = idt_entry((uint32_t) isr2, 0x08, 0x8E);
    idt[3] = idt_entry((uint32_t) isr3, 0x08, 0x8E);
    idt[4] = idt_entry((uint32_t) isr4, 0x08, 0x8E);
    idt[5] = idt_entry((uint32_t) isr5, 0x08, 0x8E);
    idt[6] = idt_entry((uint32_t) isr6, 0x08, 0x8E);
    idt[7] = idt_entry((uint32_t) isr7, 0x08, 0x8E);
    idt[8] = idt_entry((uint32_t) isr8, 0x08, 0x8E);
    idt[9] = idt_entry((uint32_t) isr9, 0x08, 0x8E);
    idt[10] = idt_entry((uint32_t) isr10, 0x08, 0x8E);
    idt[11] = idt_entry((uint32_t) isr11, 0x08, 0x8E);
    idt[12] = idt_entry((uint32_t) isr12, 0x08, 0x8E);
    idt[13] = idt_entry((uint32_t) isr13, 0x08, 0x8E);
    idt[14] = idt_entry((uint32_t) isr14, 0x08, 0x8E);
    idt[15] = idt_entry((uint32_t) isr15, 0x08, 0x8E);
    idt[16] = idt_entry((uint32_t) isr16, 0x08, 0x8E);
    idt[17] = idt_entry((uint32_t) isr17, 0x08, 0x8E);
    idt[18] = idt_entry((uint32_t) isr18, 0x08, 0x8E);
    idt[19] = idt_entry((uint32_t) isr19, 0x08, 0x8E);
    idt[20] = idt_entry((uint32_t) isr20, 0x08, 0x8E);
    idt[21] = idt_entry((uint32_t) isr21, 0x08, 0x8E);
    idt[22] = idt_entry((uint32_t) isr22, 0x08, 0x8E);
    idt[23] = idt_entry((uint32_t) isr23, 0x08, 0x8E);
    idt[24] = idt_entry((uint32_t) isr24, 0x08, 0x8E);
    idt[25] = idt_entry((uint32_t) isr25, 0x08, 0x8E);
    idt[26] = idt_entry((uint32_t) isr26, 0x08, 0x8E);
    idt[27] = idt_entry((uint32_t) isr27, 0x08, 0x8E);
    idt[28] = idt_entry((uint32_t) isr28, 0x08, 0x8E);
    idt[29] = idt_entry((uint32_t) isr29, 0x08, 0x8E);
    idt[30] = idt_entry((uint32_t) isr30, 0x08, 0x8E);
    idt[31] = idt_entry((uint32_t) isr31, 0x08, 0x8E);
    idt[32] = idt_entry((uint32_t) irq0, 0x08, 0x8E);
    idt[33] = idt_entry((uint32_t) irq1, 0x08, 0x8E);
    idt[34] = idt_entry((uint32_t) irq2, 0x08, 0x8E);
    idt[35] = idt_entry((uint32_t) irq3, 0x08, 0x8E);
    idt[36] = idt_entry((uint32_t) irq4, 0x08, 0x8E);
    idt[37] = idt_entry((uint32_t) irq5, 0x08, 0x8E);
    idt[38] = idt_entry((uint32_t) irq6, 0x08, 0x8E);
    idt[39] = idt_entry((uint32_t) irq7, 0x08, 0x8E);
    idt[40] = idt_entry((uint32_t) irq8, 0x08, 0x8E);
    idt[41] = idt_entry((uint32_t) irq9, 0x08, 0x8E);
    idt[42] = idt_entry((uint32_t) irq10, 0x08, 0x8E);
    idt[43] = idt_entry((uint32_t) irq11, 0x08, 0x8E);
    idt[44] = idt_entry((uint32_t) irq12, 0x08, 0x8E);
    idt[45] = idt_entry((uint32_t) irq13, 0x08, 0x8E);
    idt[46] = idt_entry((uint32_t) irq14, 0x08, 0x8E);
    idt[47] = idt_entry((uint32_t) irq15, 0x08, 0x8E);

    flush_idt(&idt_r);
}
