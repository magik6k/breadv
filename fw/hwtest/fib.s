.section .text
.global _start
_start:
    li x1, 0
    li x2, 1
    add x3, x1, x2

loop:
    mv x1, x2
    mv x2, x3
    add x3, x1, x2
    mv x0, x3
    mv x0, x3
    mv x0, x3
    mv x0, x3
    j loop
    nop