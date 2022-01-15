.section .text
.global _start
_start:
    # mem end
    li x1, 0x200000-4

topMsetLoop:
    sw x0, 0(x1)
    addi x1, x1, -4
    j topMsetLoop
    nop
    nop
