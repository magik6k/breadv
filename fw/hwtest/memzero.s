.section .text
.global _start
_start:
    #reloc start, size, base
    li x3, 0x200
    li x4, 0x298
    li x6, 0x200000-0x100
    mv x5, x6

    # mem end
    li x1, 0x200000-4
topMsetLoop:
    sw x0, 0(x1)
    addi x1, x1, -4
    bne x1, x4, topMsetLoop
    nop
    nop

relocLoop:
    # copy a word
    lw x1, 0(x3)
    sw x1, 0(x5)

    # see if we're done
    addi x3, x3, 4
    addi x5, x5, 4
    bne x3, x4, relocLoop
    nop

    # jump to our dest
    auipc x1, 0
    add x1, x6, x1
    addi x1, x1, 8
    jalr x0, x1
    nop
    nop

memzero:
    li x1, 0
    # mem size
    li x2, 0x200000
loop:
    sw x0, 0(x1)
    sw x0, 4(x1)
    sw x0, 8(x1)
    sw x0, 12(x1)

    addi x1, x1, 16
    bne x2, x1, loop

done:
    j done
    nop
    nop
    nop
    nop
    nop
    nop
