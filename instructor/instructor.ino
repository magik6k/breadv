const int text_cap = 7000;
unsigned char text[text_cap];

unsigned int start = 0x200;
unsigned int len = 0x204 - 0x200 + 4;
unsigned int at = 0;

inline void putWord(unsigned int w) {
  int ind = start+w;
  /*PORTA = text[ind]; // 22: 8
  PORTC = text[ind+1]; // 30: 8
  
  char b3 = text[ind+2];
  digitalWrite(38, b3&1); // port D

  digitalWrite(39, (b3&2) >> 1); // port G
  digitalWrite(40, (b3&4) >> 2);
  digitalWrite(41, (b3&8) >> 3);

  char b4 = text[ind+3];
  PORTL = ((b4 & 0x0f) << 4) && ((b3 & 0xf0) >> 4);
  PORTB = ((b4 & 0xf0) >> 4);

  //Serial.print(text[start+w], HEX);
 // Serial.print('\n');
*/

  for(int i = 22; i < 54; i++) {
     int idx = i-22;
     digitalWrite(i, (text[ind+(idx>>3)] & (1 << (idx & 0x7))) != 0);
  }
}

int clock_pin = 2;

void setup() {
  //nop
  text[0x200] = 0x13;
  text[0x201] = 0x00;
  text[0x202] = 0x00;
  text[0x203] = 0x00;
  text[0x204] = 0x13;
  text[0x205] = 0x00;
  text[0x206] = 0x00;
  text[0x207] = 0x00;

  for(int i = 22; i < 54; i++) {
    pinMode(i, OUTPUT);
  }

  putWord(0);
  
  pinMode(clock_pin, INPUT);  
  attachInterrupt(digitalPinToInterrupt(clock_pin), clk, RISING);

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  
//  for(int i = 0; i < 4; i++) {
//  for(int j = 0; j < 8; j++) {
//    text[start+i+j*4] = 1 << j;
//  }
//}  
}

/*
Serial example

> P - ping
< P - pong
> T - start transfer
(stop interrupts)
< B - ready for transfer
> [2b tx length][2b instr count (len)]
< A - ack len
> [data] <D
< D[crc32 of data] - ack
(restart interrupts)

*/

char mode = 'W';
uint16_t to_recv;

void loop() {
  if (Serial.available() == 0) {
    return;  
  }

  char sb = Serial.read();

  switch(mode) {
  case 'W':
    switch(sb) {
    case 'P': // ping
      Serial.write('P');
    break;      
    case 'T': // start transfer
      detachInterrupt(digitalPinToInterrupt(clock_pin));
      Serial.write('B');

      while(Serial.available() == 0);
      to_recv = Serial.read();
      while(Serial.available() == 0);
      to_recv += Serial.read() << 8;

      while(Serial.available() == 0);
      len = Serial.read();
      while(Serial.available() == 0);
      len += Serial.read() << 8;

      if(to_recv > text_cap) {
        attachInterrupt(digitalPinToInterrupt(clock_pin), clk, RISING);
        Serial.write('?');
        return;
      }
      Serial.write('A');

      at = 0;

      mode = 'R';
    break;      
    default:
      Serial.write('?');
    }
  break;
  case 'R':
    text[at] = sb;
    at++;
    to_recv--;
    Serial.write('D');
    if(to_recv != 0) {
      return;
    }

    /* uint32_t cs = crc.calc(text, at);
    Serial.write(); */
    at = 0;
    attachInterrupt(digitalPinToInterrupt(clock_pin), clk, RISING);
    mode = 'W';
  break;
  }
}


void clk() {
  putWord(at);
  digitalWrite(LED_BUILTIN, (at&4)>0);  
  at += 4;

  if(at >= len) {
    at = 0;
  }
}

