.section .text
.global _start
_start:
    li x1, 33
    li x3, 0x10000000
    li x3, 0x20000000
    li x3, 0x40000000

loop:
    addi x1, x1, -1
    beqz x1, _start
    li x2, 0
inner:
    beq x1, x2, loop
    addi x2, x2, 1
    j inner
