.section .boot_text

.global _start
_start:
        nop
        nop
        nop

        // sp setup
        li sp, 0x10000

        call main
        j end

end:
        auipc x6, 0
        jalr x5, x6, 0

.section .text

