#!/usr/bin/env bash

echo -e ".section .text\n.global _start\n_start:\n$1" > /tmp/s.s
riscv32-unknown-elf-gcc -nostartfiles -Tlink.ld -o a.out /tmp/s.s
riscv32-unknown-elf-objdump -D -mriscv:rv32 -d a.out | grep 200:
curl http://127.0.0.1:13333/handoff/$(riscv32-unknown-elf-objdump -D -mriscv:rv32 -d a.out | grep 200: | awk '{print $2}')