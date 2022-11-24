.section .text
.global _start
_start:
    li t0, 0x300
    li t1, 0x800

    li a0, 0xaaaaaaaa
    li a1, 0xaaaaaaaa
    li a2, 0xaaaaaaaa
    li a3, 0xaaaaaaaa
    nop
    nop

loop:
    nop
    nop
    sw a0, 0(t0)
    sw a1, 4(t0)
    sw a2, 8(t0)
    sw a3, 12(t0)
    addi x1, x0, 0x41
    addi x2, x0, 0x42
    addi x3, x0, 0x43

    addi t0, t0, 16
    bne t0, t1, loop

end:
    auipc x6, 0
    jalr x5, x6, 0
