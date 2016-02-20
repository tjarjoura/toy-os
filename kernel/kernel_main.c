#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "descriptor_table.h"
#include "lib.h"
#include "pci.h"
#include "pic.h"
#include "ps2.h"

void kernel_main() {
    init_gdt();
    init_idt();
    init_pic();
    pci_scan();
    init_ps2();

    while (1)
        ;
}
