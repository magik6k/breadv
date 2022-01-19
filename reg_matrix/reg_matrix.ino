volatile uint8_t pxdata[16*32];

const uint8_t r1r2 = 0b00000011;

const uint8_t mask_low = 0b11111010;
const uint8_t mask_hi =  0b11110101;

const uint8_t mask_g =  0b00001100;

const uint8_t shift_low = 2;
const uint8_t shift_hi = 3;

const uint8_t clk = 0b00000001;
const uint8_t stb = 0b00000010;
const uint8_t oen = 0b00000100;

const int pin_clk = 2;

const int pin_int_busy = 10;

void setup() {

Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  
  // init to red
  for(int x = 0; x < 32; x++) {
    for(int y = 0; y < 16; y++) {
      pxdata[x + (y*32)] = ((y) << 4);
      if(x == 0) {
        pxdata[x + (y*32)] |= r1r2;
      }
    }
  }

  pinMode(pin_int_busy, OUTPUT);
  digitalWrite(pin_int_busy, LOW);

  // PORTF
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);

  // PORTK
  pinMode(A8, OUTPUT);
  pinMode(A9, OUTPUT);
  pinMode(A10, OUTPUT);
  
  // input reg
  for(int i = 3; i < 9; i++) {
      pinMode(i, INPUT);
  }

  // input data
  for(int i = 22; i < 54; i++) {
      pinMode(i, INPUT);
  }

  pinMode(pin_clk, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin_clk), fclk, RISING);

  digitalWrite(LED_BUILTIN, 0);
}

void loop() {

  //fclk();

  for(uint16_t x = 0; x < 256; x+=32) {

    x += 32 * 8;

    PORTF = pxdata[x]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+1]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+2]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+3]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+4]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+5]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+6]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+7]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+8]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+9]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+10]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+11]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+12]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+13]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+14]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+15]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+16]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+17]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+18]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+19]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+20]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+21]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+22]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+23]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+24]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+25]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+26]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+27]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+28]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+29]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+30]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+31]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");  

    x -= 32*8;    

    PORTF = pxdata[x]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+1]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+2]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+3]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+4]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+5]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+6]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+7]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+8]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+9]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+10]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+11]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+12]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+13]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+14]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+15]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+16]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+17]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+18]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+19]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+20]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+21]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+22]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+23]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+24]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+25]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+26]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+27]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+28]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+29]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+30]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    PORTF = pxdata[x+31]; __asm__("nop\n\t");
    PORTK |= clk; __asm__("nop\n\t");
    PORTK &= ~clk; __asm__("nop\n\t");
    

    PORTK |= stb;
    __asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");
    PORTK &= ~stb;
    __asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");

    PORTK &= ~oen;
    delayMicroseconds(50);
    PORTK |= oen;
  }
}

uint16_t reg;
uint16_t idx;
uint16_t i;

uint8_t mask, shift;


const uint8_t shiftLUT[8] = {
    0x01,
    0x02,
    0x04,
    0x08,
    0x10,
    0x20,
    0x40,
    0x80,
};

#define GETBIT(src, bit) (((src)&(shiftLUT[(bit)]))>>(bit))

void fclk() {
  PORTB ^= 0x10;

  uint8_t A = PINA;
  uint8_t B = PINB;
  uint8_t C = PINC;
  uint8_t D = PIND;
  uint8_t G = PING;
  uint8_t L = PINL;
  uint8_t H = PINH;
  uint8_t E = PINE;

  reg = (GETBIT(H, 4)<<0) |
        (GETBIT(H, 3)<<1) |
        (GETBIT(E, 3)<<2) |
        (GETBIT(G, 5)<<3) |
        (GETBIT(E, 5)<<4);

  reg *= 32;
  idx = reg;

  mask = mask_low;
  shift = shift_low;

  if(reg >= 512) {
    idx -= 512;
    mask = mask_hi;
    shift = shift_hi;
  }

  pxdata[idx+0x00] = (pxdata[idx+0x00]&mask) | GETBIT(A, 0)<<shift;
  pxdata[idx+0x01] = (pxdata[idx+0x01]&mask) | GETBIT(A, 2)<<shift;
  pxdata[idx+0x02] = (pxdata[idx+0x02]&mask) | GETBIT(A, 4)<<shift;
  pxdata[idx+0x03] = (pxdata[idx+0x03]&mask) | GETBIT(A, 6)<<shift;
  pxdata[idx+0x04] = (pxdata[idx+0x04]&mask) | GETBIT(C, 7)<<shift;
  pxdata[idx+0x05] = (pxdata[idx+0x05]&mask) | GETBIT(C, 5)<<shift;
  pxdata[idx+0x06] = (pxdata[idx+0x06]&mask) | GETBIT(C, 3)<<shift;
  pxdata[idx+0x07] = (pxdata[idx+0x07]&mask) | GETBIT(C, 1)<<shift;
  pxdata[idx+0x08] = (pxdata[idx+0x08]&mask) | GETBIT(C, 0)<<shift;
  pxdata[idx+0x09] = (pxdata[idx+0x09]&mask) | GETBIT(C, 2)<<shift;
  pxdata[idx+0x0a] = (pxdata[idx+0x0a]&mask) | GETBIT(C, 4)<<shift;
  pxdata[idx+0x0b] = (pxdata[idx+0x0b]&mask) | GETBIT(C, 6)<<shift;
  pxdata[idx+0x0c] = (pxdata[idx+0x0c]&mask) | GETBIT(A, 7)<<shift;
  pxdata[idx+0x0d] = (pxdata[idx+0x0d]&mask) | GETBIT(A, 5)<<shift;
  pxdata[idx+0x0e] = (pxdata[idx+0x0e]&mask) | GETBIT(A, 3)<<shift;
  pxdata[idx+0x0f] = (pxdata[idx+0x0f]&mask) | GETBIT(A, 1)<<shift;
  pxdata[idx+0x10] = (pxdata[idx+0x10]&mask) | GETBIT(D, 7)<<shift;
  pxdata[idx+0x11] = (pxdata[idx+0x11]&mask) | GETBIT(G, 1)<<shift;
  pxdata[idx+0x12] = (pxdata[idx+0x12]&mask) | GETBIT(L, 7)<<shift;
  pxdata[idx+0x13] = (pxdata[idx+0x13]&mask) | GETBIT(L, 5)<<shift;
  pxdata[idx+0x14] = (pxdata[idx+0x14]&mask) | GETBIT(L, 3)<<shift;
  pxdata[idx+0x15] = (pxdata[idx+0x15]&mask) | GETBIT(L, 1)<<shift;
  pxdata[idx+0x16] = (pxdata[idx+0x16]&mask) | GETBIT(B, 3)<<shift;
  pxdata[idx+0x17] = (pxdata[idx+0x17]&mask) | GETBIT(B, 1)<<shift;
  pxdata[idx+0x18] = (pxdata[idx+0x18]&mask) | GETBIT(B, 0)<<shift;
  pxdata[idx+0x19] = (pxdata[idx+0x19]&mask) | GETBIT(B, 2)<<shift;
  pxdata[idx+0x1a] = (pxdata[idx+0x1a]&mask) | GETBIT(L, 0)<<shift;
  pxdata[idx+0x1b] = (pxdata[idx+0x1b]&mask) | GETBIT(L, 2)<<shift;
  pxdata[idx+0x1c] = (pxdata[idx+0x1c]&mask) | GETBIT(L, 4)<<shift;
  pxdata[idx+0x1d] = (pxdata[idx+0x1d]&mask) | GETBIT(L, 6)<<shift;
  pxdata[idx+0x1e] = (pxdata[idx+0x1e]&mask) | GETBIT(G, 0)<<shift;
  pxdata[idx+0x1f] = (pxdata[idx+0x1f]&mask) | GETBIT(G, 2)<<shift;

  PORTB ^= 0x10;
}
