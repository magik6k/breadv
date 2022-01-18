.section .text
.global _start
_start:
    .word 0x00000001
    .word 0x00000002
    .word 0x00000004
    .word 0x00000008
    .word 0x00000010
    .word 0x00000020
    .word 0x00000040
    .word 0x00000080
    .word 0x00000100
    .word 0x00000200
    .word 0x00000400
    .word 0x00000800
    .word 0x00001000
    .word 0x00002000
    .word 0x00004000
    .word 0x00008000

    .word 0x00010000
    .word 0x00020000
    .word 0x00040000
    .word 0x00080000
    .word 0x00100000
    .word 0x00200000
    .word 0x00400000
    .word 0x00800000
    .word 0x01000000
    .word 0x02000000
    .word 0x04000000
    .word 0x08000000
    .word 0x10000000
    .word 0x20000000
    .word 0x40000000
    .word 0x80000000

    j _start