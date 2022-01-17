.section .text
.global _start
_start:
    lw x1, c1

    j _start
    nop
    nop

.section .rodata
c1:
    .word 00000001
