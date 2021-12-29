inline void gpio_set_bank0(int c) {
	asm("csrw 0x7c0,%0"::"r"(c));
}
