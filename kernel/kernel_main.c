#include <stddef.h>
#include <stdint.h>

#include "descriptor_table.h"
#include "myos.h"
#include "pci.h"

void kernel_main() {
    init_gdt();
    init_idt();

    pci_scan();
    kprintf("hello world\n");

    asm ("int $0x40");
    while (1) ;
}
