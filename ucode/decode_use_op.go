package main

import "io/ioutil"

func main() {
	const size = 0x2000
	var out [size]byte

	var (
		op_I byte = 0b00001
		op_S byte = 0b00010
		op_B byte = 0b00100
		op_U byte = 0b01000
		op_J byte = 0b10000
	)

	inmap := map[int]byte{
		0b11100: op_I, // System (csr/e)
		0b11001: op_I, // JALR
		0b00000: op_I, // Load
		0b00100: op_I, // Arith Imm

		0b01000: op_S, // Store

		0b11000: op_B, // Branch

		0b01101: op_U, // LUI
		0b00101: op_U, // AUIPC

		0b11011: op_J, // JAL
	}

	for i := range out {
		out[i] = inmap[i&0x1f] << 3 // shift 3 for more convenient pinout
	}

	if err := ioutil.WriteFile("decode_use_op.bin", out[:], 0664); err != nil {
		panic(err)
	}
}
