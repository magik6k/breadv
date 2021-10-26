.section .text
.global _start
_start:
    li x1, 0x200

    li x2, 0x04030201

    sw x2, 0(x1)
    sw x2, 4(x1)
    sw x2, 8(x1)
    sw x2, 12(x1)

    jalr zero, 0x200(zero)
    nop
    nop
    nop
    nop
    nop
    nop
