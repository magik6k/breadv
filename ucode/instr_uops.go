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

	// FUNC BRANCH
	F_BEQ = 0b000
	F_BNE = 0b001
	F_BLT = 0b100
	F_BGE = 0b101
	F_BLTU = 0b110
	F_BGEU = 0b111

	BIT30 = 0b1_000_00000_00 // A10

	DISCARD = 0b1_0_000_00000_00 // A11

	CMP_IN = 0b1_0_0_000_00000_00 // A12

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

	BEQ = (F_BEQ << 7) | BRANCH
	BNE = (F_BNE << 7) | BRANCH
	BLT = (F_BLT << 7) | BRANCH
	BGE = (F_BGE << 7) | BRANCH
	BLTU = (F_BLTU << 7) | BRANCH
	BGEU = (F_BGEU << 7) | BRANCH
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

		u_rd byte = 0b1000_0000

		negated = u_alua_pc | u_alub_op0 | u_alu_add | u_alu_or | u_alu_xor | u_alu_and | u_rd
	)

	inmap := map[int]byte{
		LUI:   u_alub_op0 | u_alu_add | u_rd,
		AUIPC: u_alua_pc | u_alub_op0 | u_alu_add | u_rd,

		ADD:  u_alu_add | u_rd,
		ADDI: u_alub_op0 | u_alu_add | u_rd,
		XOR:  u_alu_xor | u_rd,
		XORI: u_alub_op0 | u_alu_xor | u_rd,
		OR:   u_alu_or | u_rd,
		ORI:  u_alub_op0 | u_alu_or | u_rd,
		AND:  u_alu_and | u_rd,
		ANDI: u_alub_op0 | u_alu_and | u_rd,

		STORE: u_mem_store | u_alub_op0 | u_alu_add,

		JALR: u_alua_pc | u_alu_add | u_rd, // alub +4 todo!!
		JAL: u_alua_pc | u_alu_add | u_rd, // alub +4 todo!!
	}

	spreadAllFunct := func(instr int) {
		for i := 0; i < 0b1000; i++ {
			inmap[instr|(0b001_0000000*i)] = inmap[instr]
		}
		for i := 0; i < 0b1000; i++ {
			inmap[instr|(0b001_0000000*i)|BIT30] = inmap[instr]
			inmap[instr|(0b001_0000000*i)|CMP_IN] = inmap[instr]
			inmap[instr|(0b001_0000000*i)|BIT30|CMP_IN] = inmap[instr]
		}
	}
	spreadBit30 := func(instr int) {
		inmap[instr|BIT30] = inmap[instr]
		inmap[instr|CMP_IN] = inmap[instr]
		inmap[instr|BIT30|CMP_IN] = inmap[instr]
	}

	spreadBit30(ADDI)
	spreadBit30(SLLI)
	spreadBit30(SLTI)
	spreadBit30(SLTUI)
	spreadBit30(XORI)
	spreadBit30(SRLI)
	spreadBit30(ORI)
	spreadBit30(ANDI)

	spreadBit30(ADD)
	spreadBit30(SLL)
	spreadBit30(SLT)
	spreadBit30(SLTU)
	spreadBit30(XOR)
	spreadBit30(SRL)
	spreadBit30(OR)
	spreadBit30(AND)

	spreadAllFunct(LUI)
	spreadAllFunct(AUIPC)
	spreadAllFunct(JALR)
	spreadAllFunct(JAL)
	spreadAllFunct(STORE)

	for i := range out {
		if i&DISCARD > 0 {
			out[i] = u_alu_add ^ negated
		} else {
			out[i] = inmap[i] ^ negated
		}
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

		u_cmp_lt  byte = 0b0000_0100
		u_cmp_sig byte = 0b0000_1000

		negated byte = 0
	)

	inmap := map[int]byte{
		JALR: u_pc_op | u_pc_rs1,
		JAL: u_pc_op,

		BEQ: 0,
		BNE: u_pc_op,

		BLT: u_cmp_lt,
		BGE: u_cmp_lt | u_pc_op,

		BLTU: u_cmp_sig | u_cmp_lt,
		BGEU: u_cmp_sig | u_cmp_lt | u_pc_op,

		BEQ | CMP_IN: u_pc_op,
		BNE | CMP_IN: 0,

		BLT | CMP_IN: u_cmp_lt | u_pc_op,
		BGE | CMP_IN: u_cmp_lt,

		BLTU | CMP_IN: u_cmp_sig | u_cmp_lt | u_pc_op,
		BGEU | CMP_IN: u_cmp_sig | u_cmp_lt,
	}

	spreadAllFunct := func(instr int) {
		for i := 0; i < 0b1000; i++ {
			inmap[instr|(0b001_0000000*i)] = inmap[instr]
		}
		for i := 0; i < 0b1000; i++ {
			inmap[instr|(0b001_0000000*i)|BIT30] = inmap[instr]
			inmap[instr|(0b001_0000000*i)|CMP_IN] = inmap[instr]
			inmap[instr|(0b001_0000000*i)|BIT30|CMP_IN] = inmap[instr]
		}
	}
	spreadBit30Cmp := func(instr int) {
		inmap[instr|BIT30] = inmap[instr]
		inmap[instr|CMP_IN] = inmap[instr]
		inmap[instr|BIT30|CMP_IN] = inmap[instr]
	}
	spreadBit30 := func(instr int) {
		inmap[instr|BIT30] = inmap[instr]
		inmap[instr|BIT30|CMP_IN] = inmap[instr|CMP_IN]
	}

	spreadBit30Cmp(ADDI)
	spreadBit30Cmp(SLLI)
	spreadBit30Cmp(SLTI)
	spreadBit30Cmp(SLTUI)
	spreadBit30Cmp(XORI)
	spreadBit30Cmp(SRLI)
	spreadBit30Cmp(ORI)
	spreadBit30Cmp(ANDI)

	// no add, it's sub on b30
	inmap[ADD|CMP_IN] = inmap[ADD]
	spreadBit30Cmp(SLL)
	spreadBit30Cmp(SLT)
	spreadBit30Cmp(SLTU)
	spreadBit30Cmp(XOR)
	spreadBit30Cmp(SRL)
	spreadBit30Cmp(OR)
	spreadBit30Cmp(AND)

	spreadAllFunct(LUI)
	spreadAllFunct(AUIPC)
	spreadAllFunct(JALR)
	spreadAllFunct(JAL)
	spreadAllFunct(STORE)

	spreadBit30(BEQ)
	spreadBit30(BNE)
	spreadBit30(BLT)
	spreadBit30(BGE)
	spreadBit30(BLTU)
	spreadBit30(BGEU)

	for i := range out {
		if i&DISCARD > 0 {
			out[i] = negated
		} else {
			out[i] = inmap[i] ^ negated
		}
	}

	if err := ioutil.WriteFile("instr_uops2.bin", out[:], 0664); err != nil {
		panic(err)
	}
}

func main() {
	uops1()
	uops2()
}
