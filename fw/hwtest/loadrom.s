.section .text
.global _start
_start:
    nop
    nop

    # x1 = rom addr
    # x2 = size
    # x3 = pos
    # x4-7 = scratch data

    lui x1, %hi(0x00200000) # rom base

    # load size to x2
    lw x2, 0x00(x1)
    lw x3, 0x04(x1)
    slli x3, x3, 8
    add x2, x3, x2

    addi x1, x1, 0x8
    li x3, 0


    # check done
    bge x3, x2, end

loader:
    # load byte by byte
    lw x4, 0x00(x1)
    lw x5, 0x04(x1)
    lw x6, 0x08(x1)
    lw x7, 0x0c(x1)

    # assemble word in x4
    # note: this is 1 cycle faster than 4 sb instrs, because mem acces needs 2 cycles
    slli x5, x5, 8
    slli x7, x7, 8
    or x4, x4, x5
    or x6, x6, x7
    slli x5, x6, 16
    or x4, x5, x4

    # save word
    sw x4, 0x400(x3)

    # inc counters
    addi x1, x1, 0x10
    addi x3, x3, 0x4

    # check done
    blt x3, x2, loader

end:
    j end
    nop
    nop
    nop
