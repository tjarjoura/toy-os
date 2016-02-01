.intel_syntax noprefix
.extern gdt_r

.section .text

.global flush_gdt
flush_gdt:
    mov  eax, [esp + 4]
    lgdt [eax]
    jmp  0x8:.reload_cs
.reload_cs:
    mov  ax, 0x10
    mov  ds, ax
    mov  es, ax
    mov  fs, ax
    mov  gs, ax
    mov  ss, ax
    ret

.global flush_idt
flush_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret

