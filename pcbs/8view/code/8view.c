#include<avr/io.h>
#include<avr/wdt.h>

#define D1_l1 (1<<PA0)
#define D1_l6 (1<<PA1)
#define D1_l2 (1<<PA2)
#define D1_l7 (1<<PA3)
#define D1_l3 (1<<PA4)
#define D1_l5 (1<<PA5)
#define D1_l8 (1<<PA6)
#define D1_l4 (1<<PA7)

#define D2_l7 (1<<PB0)
#define D2_l8 (1<<PB1)
#define D2_l5 (1<<PB2)
#define D2_l4 (1<<PB3)
#define D2_l6 (1<<PB4)
#define D2_l3 (1<<PB5)
#define D2_l1 (1<<PB6)
#define D2_l2 (1<<PB7)

#define D1_A D1_l1
#define D1_B D1_l2
#define D1_C D1_l3
#define D1_D D1_l4
#define D1_E D1_l5
#define D1_F D1_l6
#define D1_G D1_l7
#define D1_DP D1_l8

#define D2_A D2_l1
#define D2_B D2_l2
#define D2_C D2_l3
#define D2_D D2_l4
#define D2_E D2_l5
#define D2_F D2_l6
#define D2_G D2_l7
#define D2_DP D2_l8

#define D1_c1 D1_B|D1_C|0
#define D1_c2 D1_A|D1_B|D1_D|D1_E|D1_G|0
#define D1_c3 D1_A|D1_B|D1_C|D1_D|D1_G|0
#define D1_c4 D1_B|D1_C|D1_F|D1_G|0
#define D1_c5 D1_A|D1_C|D1_D|D1_F|D1_G|0
#define D1_c6 D1_A|D1_C|D1_D|D1_E|D1_F|D1_G|0
#define D1_c7 D1_A|D1_B|D1_C|0
#define D1_c8 D1_A|D1_B|D1_C|D1_D|D1_E|D1_F|D1_G|0
#define D1_c9 D1_A|D1_B|D1_C|D1_D|D1_F|D1_G|0
#define D1_c0 D1_A|D1_B|D1_C|D1_D|D1_E|D1_F|0
#define D1_cA D1_A|D1_B|D1_C|D1_D|D1_E|D1_G|0
#define D1_cB D1_C|D1_D|D1_E|D1_F|D1_G|0
#define D1_cC D1_D|D1_E|D1_G|0
#define D1_cD D1_B|D1_C|D1_D|D1_E|D1_G|0
#define D1_cE D1_A|D1_D|D1_E|D1_F|D1_G|0
#define D1_cF D1_A|D1_E|D1_F|D1_G|0
#define D1_cG D1_A|D1_C|D1_D|D1_E|D1_F|0
#define D1_cH D1_C|D1_E|D1_F|D1_G|0
#define D1_cI D1_A|D1_E|0
#define D1_cJ D1_A|D1_C|D1_D|0
#define D1_cK D1_A|D1_C|D1_E|D1_F|D1_G|0
#define D1_cL D1_D|D1_E|D1_F|0
#define D1_cM D1_A|D1_C|D1_E|D1_G|0
#define D1_cN D1_C|D1_E|D1_G|0
#define D1_cO D1_C|D1_D|D1_E|D1_G|0
#define D1_cP D1_A|D1_B|D1_E|D1_F|D1_G|0
#define D1_cQ D1_A|D1_B|D1_C|D1_F|D1_G|0
#define D1_cR D1_E|D1_G|0
#define D1_cS D1_A|D1_C|D1_D|D1_F|0
#define D1_cT D1_D|D1_E|D1_F|D1_G|0
#define D1_cU D1_C|D1_D|D1_E|0
#define D1_cV D1_B|D1_D|D1_F|0
#define D1_cW D1_B|D1_D|D1_F|D1_G|0
#define D1_cX D1_C|D1_E|0
#define D1_cY D1_B|D1_C|D1_D|D1_F|D1_G|0
#define D1_cZ D1_A|D1_B|D1_D|D1_E|0

#define D2_c1 D2_B|D2_C|0
#define D2_c2 D2_A|D2_B|D2_D|D2_E|D2_G|0
#define D2_c3 D2_A|D2_B|D2_C|D2_D|D2_G|0
#define D2_c4 D2_B|D2_C|D2_F|D2_G|0
#define D2_c5 D2_A|D2_C|D2_D|D2_F|D2_G|0
#define D2_c6 D2_A|D2_C|D2_D|D2_E|D2_F|D2_G|0
#define D2_c7 D2_A|D2_B|D2_C|0
#define D2_c8 D2_A|D2_B|D2_C|D2_D|D2_E|D2_F|D2_G|0
#define D2_c9 D2_A|D2_B|D2_C|D2_D|D2_F|D2_G|0
#define D2_c0 D2_A|D2_B|D2_C|D2_D|D2_E|D2_F|0
#define D2_cA D2_A|D2_B|D2_C|D2_D|D2_E|D2_G|0
#define D2_cB D2_C|D2_D|D2_E|D2_F|D2_G|0
#define D2_cC D2_D|D2_E|D2_G|0
#define D2_cD D2_B|D2_C|D2_D|D2_E|D2_G|0
#define D2_cE D2_A|D2_D|D2_E|D2_F|D2_G|0
#define D2_cF D2_A|D2_E|D2_F|D2_G|0
#define D2_cG D2_A|D2_C|D2_D|D2_E|D2_F|0
#define D2_cH D2_C|D2_E|D2_F|D2_G|0
#define D2_cI D2_A|D2_E|0
#define D2_cJ D2_A|D2_C|D2_D|0
#define D2_cK D2_A|D2_C|D2_E|D2_F|D2_G|0
#define D2_cL D2_D|D2_E|D2_F|0
#define D2_cM D2_A|D2_C|D2_E|D2_G|0
#define D2_cN D2_C|D2_E|D2_G|0
#define D2_cO D2_C|D2_D|D2_E|D2_G|0
#define D2_cP D2_A|D2_B|D2_E|D2_F|D2_G|0
#define D2_cQ D2_A|D2_B|D2_C|D2_F|D2_G|0
#define D2_cR D2_E|D2_G|0
#define D2_cS D2_A|D2_C|D2_D|D2_F|0
#define D2_cT D2_D|D2_E|D2_F|D2_G|0
#define D2_cU D2_C|D2_D|D2_E|0
#define D2_cV D2_B|D2_D|D2_F|0
#define D2_cW D2_B|D2_D|D2_F|D2_G|0
#define D2_cX D2_C|D2_E|0
#define D2_cY D2_B|D2_C|D2_D|D2_F|D2_G|0
#define D2_cZ D2_A|D2_B|D2_D|D2_E|0

const uint8_t d1lut[] = {
    D1_c0,
    D1_c1,
    D1_c2,
    D1_c3,
    D1_c4,
    D1_c5,
    D1_c6,
    D1_c7,
    D1_c8,
    D1_c9,
    D1_cA,
    D1_cB,
    D1_cC,
    D1_cD,
    D1_cE,
    D1_cF
};

const uint8_t d2lut[] = {
    D2_c0,
    D2_c1,
    D2_c2,
    D2_c3,
    D2_c4,
    D2_c5,
    D2_c6,
    D2_c7,
    D2_c8,
    D2_c9,
    D2_cA,
    D2_cB,
    D2_cC,
    D2_cD,
    D2_cE,
    D2_cF
};

// <riscv instruction mode>

#define RV_INSTR_MODE 0

// riscv instructions, opcode (bits 6:2) and funct3 (bits 14:12)
#define rLU ((0<<6)|(1<<5)|(1<<4)|(0<<3)|(1<<2)) >> 2 // LUI
#define rAU ((0<<6)|(0<<5)|(1<<4)|(0<<3)|(1<<2)) >> 2 // AUIPC
#define rJL ((1<<6)|(1<<5)|(0<<4)|(1<<3)|(1<<2)) >> 2 // JAL
#define rJR ((1<<6)|(1<<5)|(0<<4)|(0<<3)|(1<<2)) >> 2 // JALR
#define rBR ((1<<6)|(1<<5)|(0<<4)|(0<<3)|(0<<2)) >> 2 // BEQ
#define rLD ((0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)) >> 2 // LB
#define rSD ((0<<6)|(1<<5)|(0<<4)|(0<<3)|(0<<2)) >> 2 // SB
#define rAI ((0<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)) >> 2 // ADDI
#define rAR ((0<<6)|(1<<5)|(1<<4)|(0<<3)|(0<<2)) >> 2 // ADD
#define rFE ((0<<6)|(0<<5)|(0<<4)|(1<<3)|(1<<2)) >> 2 // FENCE
#define rEE ((1<<6)|(1<<5)|(1<<4)|(0<<3)|(0<<2)) >> 2 // ECALL
#define rSY ((1<<6)|(1<<5)|(1<<4)|(0<<3)|(0<<2)) >> 2 // EBREAK

const uint8_t rviLutD1[32] = {
    [rLU] = D1_cL,
    [rAU] = D1_cA,
    [rJL] = D1_cJ,
    [rJR] = D1_cJ,
    [rBR] = D1_cB,
    [rLD] = D1_cL,
    [rSD] = D1_cS,
    [rAI] = D1_cA,
    [rAR] = D1_cA,
    [rFE] = D1_cF,
    [rEE] = D1_cE,
    [rSY] = D1_cS
};

// 5 bits of opcode
const uint8_t rviLutD2[32] = {
    [rLU] = D2_cU,
    [rAU] = D2_cU,
    [rJL] = D2_cL,
    [rJR] = D2_cR,
    [rBR] = D2_cR,
    [rLD] = D2_cD,
    [rSD] = D2_cD,
    [rAI] = D2_cI,
    [rAR] = D2_cR,
    [rFE] = D2_cE,
    [rEE] = D2_cE,
    [rSY] = D2_cY
};

#define getbit(i, src, dst) (((i&(1<<src))>>src)<<dst)

void main() {
    DDRB |= 0xff;
    DDRA |= 0xff;
    DDRC |= 0x0;

    PORTA = 0xff;
    PORTB = 0xff;

    while(1) {
            wdt_reset();

            uint8_t val = PINC;

            uint8_t instr = getbit(val, 7, 0) | getbit(val, 6, 1) | getbit(val, 5, 2) | getbit(val, 4, 3) | getbit(val, 3, 4) | getbit(val, 2, 5) | getbit(val, 1, 6);

            #if RV_INSTR_MODE
            // if non zero in lut, use it with D2_DP set, otherwise fallback to default display
            if(rviLutD1[(instr>>2)] && instr&0x3 > 0){
                instr = instr >> 2;
                instr = instr & 0x1f;

                PORTA = ~(rviLutD1[instr]);
                if getbit(val, 0, 7) {
                    PORTA ^= D1_DP;
                }
                PORTB = ~(rviLutD2[instr] | D2_DP);

                continue;
            }

            #endif

            instr |= getbit(val, 0, 7);

            PORTA = ~d1lut[(instr >> 4) & 0xf];
            PORTB = ~d2lut[instr & 0xf];

    }
}