#include "common.h"

/* EXTERNAL FUNCTIONS */

/* Loads a new global descriptor table */
void flush_gdt(void *gdt_r);

/* An 8-byte entry in the GDT, describing a memory segment */
struct gdt_entry {
    uint16_t limit;
    uint16_t base_lo;
    uint8_t base_mi;
    uint8_t access;
    uint8_t flags;
    uint8_t base_hi;
} __attribute__((packed));

/* The entries in here define segments of the system memory map. */
struct gdt_entry gdt[3];

/* This will return an 8-byte GDT entry corresponding to the input attributes */
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

struct {
    uint16_t size;
    struct gdt_entry *gdt_addr;
} __attribute__((packed))
gdt_r = { .size = sizeof(gdt), .gdt_addr = gdt};

void init_gdt() {
    /*                   base        limit    access flags */
    gdt[0] = gdt_entry(0x00000000, 0x00000000, 0x00, 0x00); /* Null descriptor */
    gdt[1] = gdt_entry(0x00000000, 0x000FFFFF, 0x9A, 0xCF); /* Code */
    gdt[2] = gdt_entry(0x00000000, 0x000FFFFF, 0x92, 0xCF); /* Data */
    flush_gdt(&gdt_r);
}

