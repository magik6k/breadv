.section .boot_text

.global _start
_start:
        nop
        nop
        nop
        call reset
        call selftest
_test_ret:
        call reset

        # sp setup
        li sp, 0x10000

        call main
        j end

end:
        auipc x6, 0
        jalr x5, x6, 0

reset:
    li x0,0
    li x2,0
    li x3,0
    li x4,0
    li x5,0
    li x6,0
    li x7,0
    li x8,0
    li x9,0
    li x10,0
    li x11,0
    li x12,0
    li x13,0
    li x14,0
    li x15,0
    li x16,0
    li x17,0
    li x18,0
    li x19,0
    li x20,0
    li x21,0
    li x22,0
    li x23,0
    li x24,0
    li x25,0
    li x26,0
    li x27,0
    li x28,0
    li x29,0
    li x30,0
    li x31,0
    ret

selftest:
    li x31, 0x00ff0000

test_propagate:
    li x2, 0xf040a355

    mv x3,x2
    mv x4,x3
    mv x5,x4
    mv x6,x5
    mv x7,x6
    mv x8,x7
    mv x9,x8
    mv x10,x9
    mv x11,x10
    mv x12,x11
    mv x13,x12
    mv x14,x13
    mv x15,x14
    mv x16,x15
    mv x17,x16
    mv x18,x17
    mv x19,x18
    mv x20,x19
    mv x21,x20
    mv x22,x21
    mv x23,x22
    mv x24,x23
    mv x25,x24
    mv x26,x25
    mv x27,x26
    mv x28,x27
    mv x29,x28
    mv x30,x29

    li x31, 0xff01

    bne x2, x30, end

.sum_prog:
    call reset

    // a0 = start, a1 = end

    li a0, 0
    li a1, 0x47c

    // t6 = accumulator
    mv t6, s8

loop:
    // a2 = data
    lw a2, 0(a0)
    addi a0, a0, 4

    // sh[a3] = (acc[t6]^data[a2]) & 0x7 + 1
    xor a3, t6, a2
    andi a3, a3, 0x7
    addi a3, a3, 1

    // acc[t6] = data[a2] + [t6]([a4](acc[t6] >> sh[a3]) ^ acc) | [a5](acc << 7)
    srl a4, t6, a3
    slli a5, t6, 7
    xor t6, t6, a4
    add t6, t6, a2
    xor t6, t6, a5

    // acc = acc ^ c[[a3](acc&0xc)]
    andi a3, t6, 0xc
    add a3, a3, %lo(sum_consts)
    lw a3, 0(a3)
    xor t6, t6, a3

    bne a0, a1, loop

    j _test_ret

sum_consts:
    .word 0xf2d20da2
    .word 0x11a69beb
    .word 0xd44c26c1
    .word 0x73758a62

.section .text

