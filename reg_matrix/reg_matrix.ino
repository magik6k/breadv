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

void setup() {
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

void fclk() {
  reg = 0;
  
  for(i = 3; i <= 8; i++) {
     idx = i-3;

     if(digitalRead(i)) {        
       reg |= (1 << idx);
     }
  }

  reg *= 32;

  if(reg < 512) {
    for(i = 22; i < 54; i++) {
      idx = i-22+reg;

      pxdata[idx] = (pxdata[idx] & mask_low) | (digitalRead(i) << shift_low)&mask_g;
    }
  } else {
    reg -= 512;

    for(i = 22; i < 54; i++) {
      idx = i-22+reg;

      pxdata[idx] = (pxdata[idx] & mask_hi) | (digitalRead(i) << shift_hi)&mask_g;
    }
  }
}
