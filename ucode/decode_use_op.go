package main

import "io/ioutil"

func main() {
	const size = 0x2000
	var out [size]byte

	var (
		// out
		op_I byte = 0b00001_000
		op_S byte = 0b00010_000
		op_B byte = 0b00100_000
		op_U byte = 0b01000_000
		op_J byte = 0b10000_000

		rs1_disable byte = 0b00000_001
		rs2_disable byte = 0b00000_010

		// in
		rs1_zero = 0b01_000_00000
		rs2_zero = 0b10_000_00000
	)

	opmap := map[int]byte{
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

	rs2map := map[int]byte{
		0b11000: rs2_disable,
		0b01000: rs2_disable,
		0b01100: rs2_disable,
	}

	rs1amap := map[int]byte{
		0b01101: rs1_disable,
		0b00101: rs1_disable,
		0x11011: rs1_disable,
	}

	rs1fmap := map[int]byte{
		0b000_11100: rs1_disable,
		0b101_11100: rs1_disable,
		0b110_11100: rs1_disable,
		0b111_11100: rs1_disable,
	}

	for i := range out {
		// write ops first
		out[i] = opmap[i&0x1f]

		// rs2
		out[i] |= rs2map[i&0x1f]^rs2_disable
		if i&rs2_zero > 0 {
			out[i] |= rs2_disable
		}

		// rs1
		out[i] |= rs1amap[i&0x1f]
		out[i] |= rs1fmap[i&0x7f]
		if i&rs1_zero > 0 {
			out[i] |= rs1_disable
		}

	}

	if err := ioutil.WriteFile("decode_use_op.bin", out[:], 0664); err != nil {
		panic(err)
	}
}
