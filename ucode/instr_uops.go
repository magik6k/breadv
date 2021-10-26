package main

import "io/ioutil"

const (
	LUI    = 0b01101_11
	AUIPC  = 0b00101_11
	JAL    = 0b11011_11
	JALR   = 0b11001_11
	BRANCH = 0b11000_11
	LOAD   = 0b00000_11
	STORE  = 0b01000_11
	ARITHI = 0b00100_11
	ARITH  = 0b01100_11
	SYSTEM = 0b11100_11

	// FUNC ARITH
	F_ADD  = 0b000
	F_SLL  = 0b001
	F_SLT  = 0b010
	F_SLTU = 0b011
	F_XOR  = 0b100
	F_SRL  = 0b101
	F_OR   = 0b110
	F_AND  = 0b111

	BIT30 = 0b1_000_00000_00

	ADD = (F_ADD << 7) | ARITH
	SLL = (F_SLL << 7) | ARITH
	SLT = (F_SLT << 7) | ARITH
	SLTU = (F_SLTU << 7) | ARITH
	XOR = (F_XOR << 7) | ARITH
	SRL = (F_SRL << 7) | ARITH
	OR = (F_OR << 7) | ARITH
	AND = (F_AND << 7) | ARITH
	SUB  = ADD | BIT30 // non-i, bit 30=1
	SRA  = SRL | BIT30  // bit 30=1

	ADDI = (F_ADD << 7) | ARITHI
	SLLI = (F_SLL << 7) | ARITHI
	SLTI = (F_SLT << 7) | ARITHI
	SLTUI = (F_SLTU << 7) | ARITHI
	XORI = (F_XOR << 7) | ARITHI
	SRLI = (F_SRL << 7) | ARITHI
	ORI = (F_OR << 7) | ARITHI
	ANDI = (F_AND << 7) | ARITHI
	// SRAI?
)



func uops1() {
	const size = 0x2000
	var out [size]byte

	var (
		u_alua_pc  byte = 0b0000_0001 // a=op1 otherwise
		u_alub_op0 byte = 0b0000_0010 // b=op2 otherwise

		u_alu_add byte = 0b0000_0100
		u_alu_or byte  = 0b0000_1000
		u_alu_xor byte = 0b0001_0000
		u_alu_and byte = 0b0010_0000

		u_mem_store byte = 0b0100_0000

		negated = u_alua_pc | u_alub_op0 | u_alu_add | u_alu_or | u_alu_xor | u_alu_and
	)

	inmap := map[int]byte{
		LUI:   u_alub_op0 | u_alu_add,
		AUIPC: u_alua_pc | u_alub_op0 | u_alu_add,

		ADD:  u_alu_add,
		ADDI: u_alub_op0 | u_alu_add,

		STORE: u_mem_store | u_alub_op0 | u_alu_add,

		JALR: u_alua_pc | u_alu_add, // alub +4 todo!!
		JAL: u_alua_pc | u_alu_add, // alub +4 todo!!
	}

	spreadAllFunct := func(instr int) {
		for i := 0; i < 0b1000; i++ {
			inmap[instr|(0b001_0000000*i)] = inmap[instr]
		}
		for i := 0; i < 0b1000; i++ {
			inmap[instr|(0b001_0000000*i)|BIT30] = inmap[instr]
		}
	}
	spreadBit30 := func(instr int) {
		inmap[instr|BIT30] = inmap[instr]
	}

	spreadBit30(ADDI)
	spreadBit30(SLLI)
	spreadBit30(SLTI)
	spreadBit30(SLTUI)
	spreadBit30(XORI)
	spreadBit30(SRLI)
	spreadBit30(ORI)
	spreadBit30(ANDI)

	spreadAllFunct(LUI)
	spreadAllFunct(AUIPC)
	spreadAllFunct(JALR)
	spreadAllFunct(JAL)
	spreadAllFunct(STORE)

	for i := range out {
		out[i] = inmap[i] ^ negated
	}

	if err := ioutil.WriteFile("instr_uops1.bin", out[:], 0664); err != nil {
		panic(err)
	}
}

func uops2() {
	const size = 0x2000
	var out [size]byte

	var (
		u_pc_op  byte = 0b0000_0001 // +4 otherwise
		u_pc_rs1 byte = 0b0000_0010 // +pc otherwise

		negated byte = 0
	)

	inmap := map[int]byte{
		JALR: u_pc_op | u_pc_rs1,
		JAL: u_pc_op,
	}

	spreadAllFunct := func(instr int) {
		for i := 0; i < 0b1000; i++ {
			inmap[instr|(0b001_0000000*i)] = inmap[instr]
		}
		for i := 0; i < 0b1000; i++ {
			inmap[instr|(0b001_0000000*i)|BIT30] = inmap[instr]
		}
	}
	spreadBit30 := func(instr int) {
		inmap[instr|BIT30] = inmap[instr]
	}

	spreadBit30(ADDI)
	spreadBit30(SLLI)
	spreadBit30(SLTI)
	spreadBit30(SLTUI)
	spreadBit30(XORI)
	spreadBit30(SRLI)
	spreadBit30(ORI)
	spreadBit30(ANDI)

	spreadAllFunct(LUI)
	spreadAllFunct(AUIPC)
	spreadAllFunct(JALR)
	spreadAllFunct(JAL)
	spreadAllFunct(STORE)

	for i := range out {
		out[i] = inmap[i] ^ negated
	}

	if err := ioutil.WriteFile("instr_uops2.bin", out[:], 0664); err != nil {
		panic(err)
	}
}

func main() {
	uops1()
	uops2()
}
