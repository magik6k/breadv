.section .boot_text

.globl _start
_start:
        # sp setup
        addi sp, zero, 0x1f0

        call main
        j end

end:
        auipc x6, 0
        jalr x5, x6, 0

.section .text