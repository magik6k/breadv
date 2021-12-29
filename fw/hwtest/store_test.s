.section .text
.global _start
_start:
    li x1, 0x00

    li x2, 0x08040201

    sw x2, 0(x1)
    sw x2, 4(x1)
    sw x2, 8(x1)
    sw x2, 12(x1)

    li x2, 0

loop:
    j loop
    nop
    nop
    nop
    nop
    nop
    nop
