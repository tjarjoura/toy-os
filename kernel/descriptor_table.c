#include <stdint.h>
#include "descriptor_table.h"
#include "myos.h"
#include "sysio.h"

void flush_gdt(void *);
void flush_idt(void *);

/* Instruction Service Routines */
void isr0();
void isr1();
void isr2();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();

struct gdt_entry {
    uint16_t limit;
    uint16_t base_lo;
    uint8_t base_mi;
    uint8_t access;
    uint8_t flags;
    uint8_t base_hi;
} __attribute__((packed));

struct gdt_entry gdt[3];   /* Global Descriptor Table */

struct idt_entry {
    uint16_t offset_lo;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t offset_hi;
} __attribute__((packed));

struct idt_entry idt[256]; /* Interrupt Descriptor Table */

struct {
    uint16_t size;
    struct gdt_entry *gdt_addr;
} __attribute__((packed))
gdt_r = { .size = sizeof(gdt), .gdt_addr = gdt};

struct {
    uint16_t size;
    struct idt_entry *idt_addr;
} __attribute__((packed))
idt_r = { .size = sizeof(idt), .idt_addr = idt};

static struct gdt_entry gdt_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    struct gdt_entry entry;
    entry.base_hi = (base >> 24) & 0xFF;
    entry.base_mi = (base >> 16) & 0xFF;
    entry.base_lo = base & 0x0000FFFF;

    entry.access = access;
    entry.flags = (flags & 0xF0) | ((limit >> 16) & 0x0F);

    entry.limit = limit & 0x0000FFFF;

    return entry;
}

static struct idt_entry idt_entry(uint32_t offset, uint16_t selector, uint8_t flags) {
    struct idt_entry entry;

    entry.offset_lo = 0x0000FFFF & offset;
    entry.offset_hi = (0xFFFF0000 & offset) >> 16;

    entry.selector = selector;
    entry.flags = flags;

    entry.zero = 0x00;

    return entry;
}

void init_gdt() {
    /*                   base        limit    access flags */
    gdt[0] = gdt_entry(0x00000000, 0x00000000, 0x00, 0x00); /* Null descriptor */
    gdt[1] = gdt_entry(0x00000000, 0x000FFFFF, 0x9A, 0xCF); /* Code */
    gdt[2] = gdt_entry(0x00000000, 0x000FFFFF, 0x92, 0xCF); /* Data */
    flush_gdt(&gdt_r);
}

struct saved_registers {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha */
    uint32_t int_num, err_code;
};

void interrupt_handler(struct saved_registers saved_registers) {
    vga_putstring("Received interrupt ");
    vga_putchar(saved_registers.int_num + '0');
    vga_putchar('\n');

    vga_putstring("Error code ");
    vga_putchar(saved_registers.err_code + '0');
    vga_putchar('\n');
}

void irq_handler(struct saved_registers saved_registers) {
    if (saved_registers.int_num > 40) /* Interrupt came from Slave PIC */
        outb(0xA0, 0x20);
    outb(0x20, 0x20); /* Send EOI signal to PIC controllers */

    kprintf("Received Interrupt(IRQ): %d\n", saved_registers.int_num);
}

void init_idt() {
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

    outb(0x43, 0x36);
    outb(0x40, 0xFF);
    outb(0x40, 0x00);
}
