.section .text

.globl _start

_start:
        # sp setup
        addi sp, zero, 0x1f0

        call main

end:
        auipc x6, 0
        jalr x5, x6, 0
