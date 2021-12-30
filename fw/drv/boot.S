.section .text
.globl _start

_start:
	# sp setup
	li sp, 0x10000

	call main
end:
	auipc x6, 0
	jalr x5, x6, 0
