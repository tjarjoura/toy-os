#include <stdint.h>
#include "descriptor_table.h"

extern void flush_gdt(void *);

struct gdt_entry {
    uint8_t base_hi;
    uint8_t flags;
    uint8_t access;
    uint8_t base_mi;
    uint16_t base_lo;
    uint16_t limit;
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

static struct gdt_entry gdt_entry(uint32_t base, uint32_t limit, uint8_t flags, uint8_t access) {
    struct gdt_entry entry;
    entry.base_hi = base & 0xFF000000;
    entry.base_mi = base & 0x00FF0000;
    entry.base_lo = base & 0x0000FFFF;

    entry.access = access;
    entry.flags = (flags & 0xFF00) | ((limit >> 16) & 0x00FF);

    entry.limit = limit & 0x0000FFFF;

    return entry;
}

static struct idt_entry idt_entry(uint32_t offset, uint16_t seletor, uint8_t flags) {
    struct idt_entry entry;

    return entry;
}

void init_descriptor_tables() {
    /* Null Descriptor */
    gdt[0] = gdt_entry(0x00000000, 0x00000000, 0x00, 0x00);
    /* Code */
    gdt[1] = gdt_entry(0x00000000, 0x000FFFFF, 0x9A, 0xCF); /* 1001 1010 */
    /* Data */
    gdt[2] = gdt_entry(0x00000000, 0x000FFFFF, 0x92, 0xCF);
    flush_gdt(&gdt_r);
}

