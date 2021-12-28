.section .text
.global _start
_start:
    nop
    nop
    nop
    nop
    li x1, 0b0011
    li x2, 0b1001
loop:
    add x0, x1, x2
    and x0, x1, x2
    or x0, x1, x2
    xor x0, x1, x2
    j loop
    nop
    nop
    nop

