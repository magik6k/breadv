const int text_cap = 1000;
volatile unsigned char text[text_cap];

volatile unsigned char ex_temp[8];
volatile bool run_temp = false;
volatile bool toggle_handoff = false;
volatile bool in_handoff = false;

volatile unsigned int start = 0x200;
volatile unsigned int len = 0x204 - 0x200 + 4;
volatile unsigned int at = 0;

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

inline void putTemp() {
  for(int i = 22; i < 54; i++) {
     int idx = i-22;
     digitalWrite(i, (ex_temp[(idx>>3)] & (1 << (idx & 0x7))) != 0);
  }
}

int clock_pin = 2;
int handoff_pin = 3;

void setupNop() {
    text[0x200] = 0x13;
    text[0x201] = 0x00;
    text[0x202] = 0x00;
    text[0x203] = 0x00;
    text[0x204] = 0x13;
    text[0x205] = 0x00;
    text[0x206] = 0x00;
    text[0x207] = 0x00;
    len = 0x204 - 0x200 + 4;
    at = 0;
}

void setupOutput() {
    in_handoff = false;
    digitalWrite(handoff_pin, HIGH);
    for(int i = 22; i < 54; i++) {
        pinMode(i, OUTPUT);
    }
}

void setupHandoff() {
    for(int i = 22; i < 54; i++) {
        pinMode(i, INPUT);
    }
    in_handoff = true;
    digitalWrite(handoff_pin, LOW);
}

void setup() {
  pinMode(handoff_pin, OUTPUT);

  //nop
  setupNop();
  setupOutput();

  putWord(0);
  
  pinMode(clock_pin, INPUT);  
  attachInterrupt(digitalPinToInterrupt(clock_pin), clk, CHANGE);

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200, SERIAL_8O1);
  
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

> X - execute single
> [4b instr]
< A - ack
...
< D - executed

> Y - handoff
> [4b instr]
< A - ack
...
< - D - handed off

> P - ping
< P - pong, exit handoff



*/

// W = wait
// R = receive
// Y = yield execution
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
    case 'Y': // handoff
    case 'X': // execute
        setupNop();
        run_temp = false;

        Serial.write('R'); // read

        while(Serial.available() == 0);
        ex_temp[7] = Serial.read();
        while(Serial.available() == 0);
        ex_temp[6] = Serial.read();
        while(Serial.available() == 0);
        ex_temp[5] = Serial.read();
        while(Serial.available() == 0);
        ex_temp[4] = Serial.read();

        Serial.write('A'); // read ack

        // wait for low clock
        while(digitalRead(clock_pin));

        ex_temp[0] = ex_temp[4];
        ex_temp[1] = ex_temp[5];
        ex_temp[2] = ex_temp[6];
        ex_temp[3] = ex_temp[7];
        toggle_handoff = sb == 'Y';
        run_temp = true;

        while(run_temp || toggle_handoff);
        Serial.write('D'); // exec ack

        if(sb == 'Y') {
            mode = 'Y';
        }
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
    mode = 'W';
  break;
  case 'Y':
    switch(sb) {
    case 'P': // ping; exit handoff
      toggle_handoff = true;
      while(toggle_handoff);

      Serial.write('P');
      mode = 'W';
    break;
    default:
      Serial.write('?');
    }
  break;
  }
}

void clk() {
  bool pin = digitalRead(clock_pin);
  if(!pin) {
    return;
  }

  if(in_handoff) {
    if(!toggle_handoff) {
        return;
    }

    setupOutput();
    toggle_handoff = false;
  }

  if(run_temp) {
    putTemp();
    digitalWrite(LED_BUILTIN, 1);
    run_temp = false;
    return;
  }
  if(toggle_handoff) {
    setupHandoff();
    toggle_handoff = false;
    return;
  }

  putWord(at);
  digitalWrite(LED_BUILTIN, 0);
  at += 4;

  if(at >= len) {
    at = 0;
  }
}

