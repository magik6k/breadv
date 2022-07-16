.section .boot_text

.global _start
_start:
        nop
        nop
        nop
        call reset

        # sp setup
        li sp, 0x10000



        j end

end:
        auipc x6, 0
        jalr x5, x6, 0
        j end
        j end
        j end
        j end

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

.section .text

# T3-T6 reverved for tests
# T6 = testID

# test 0x01
test_adds:
    li t6, 0x1001

    li a0, 0x10
    li a1, 0x10

    add a1, a1, a0
    add a1, a1, a0
    add a1, a1, a0
    add a1, a1, a0
    add a1, a1, a0
    add a1, a1, a0
    add a1, a1, a0

    li a3, 0x80

    bne a1, a3, end

test_bnz:

test_done:
    li t6, 0xffff
    j end


