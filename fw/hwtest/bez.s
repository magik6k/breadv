.section .text
.global _start
_start:
    li x1, 32
    li x2, 0x10000000
    li x2, 0x20000000
    li x2, 0x40000000

loop:
    addi x1, x1, -1
    bnez x1, loop
    j _start
