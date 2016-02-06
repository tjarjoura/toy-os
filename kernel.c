#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "myos.h"
#include "descriptor_table.h"

void kernel_main() {
    init_gdt();
    init_idt();

    uint8_t *msg = "This doesn't do anything yet.\n"
                   "[-----------------------------]\n"
                   "[-----------------------------]\n"
                   "[-------------x-----x---------]\n"
                   "[------------x-xxxxx----------]\n"
                   "[------------xxx--------------]\n"
                   "[-----------------------------]\n"
                   "[-----------------------------]\n"
                   "[-----------------------------]\n"
                   "[-----------------------------]\n"
                   "Copyright 2016 Tyler Jarjoura\n\n\n";

    vga_putstring(msg);
    asm ("int $0");
    asm ("int $1");
    asm ("int $2");
    asm ("int $3");
    asm ("int $4");
    asm ("int $5");
    asm ("int $6");
    asm ("int $7");
    asm ("int $8");
    /*
    asm ("int $9");
    asm ("int $10");
    asm ("int $11");
    asm ("int $12");
    asm ("int $13");
    asm ("int $14");
    asm ("int $15");
    asm ("int $16");
    asm ("int $17");
    asm ("int $18");
    asm ("int $19");
    asm ("int $20");
    asm ("int $21");
    asm ("int $22");
    asm ("int $23");
    asm ("int $24");
    asm ("int $25");
    asm ("int $26");
    asm ("int $27");
    asm ("int $28");
    asm ("int $29");
    asm ("int $30");
    asm ("int $31");
    asm ("int $32");
    asm ("int $33");
    asm ("int $34");
    asm ("int $35");
    asm ("int $36");
    */

    while (1) ;

}
