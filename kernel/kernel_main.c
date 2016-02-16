#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "descriptor_table.h"
#include "lib.h"
#include "pci.h"

void kernel_main() {
    init_gdt();
    init_idt();

    pci_scan();
    kprintf("hello world\n");
    asm volatile ("int $40");

    while (1) ;
}
