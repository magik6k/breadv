.section .text
.global _start
_start:
    li x1, 0x00
    li x2, 0x00
    li x3, 0x08040201

loop:

    sw x3, 0(x1)

    nop
    nop
    nop

    lw x2, 0(x1)
    add x0, x0, x2
    add x0, x0, x2
    add x0, x0, x2

    j loop
    nop
    nop
    nop
    nop
    nop
    nop