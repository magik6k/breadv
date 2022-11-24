volatile unsigned char ex_temp[8];
volatile bool run_temp = false;
volatile bool toggle_handoff = false;
volatile bool in_handoff = false;

volatile bool start_dumping = false;
volatile bool dumping = false;
volatile bool dump_next = false;
volatile bool dump_done = false;
volatile int32_t to_dump = 0;

inline void putTemp() {

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
     digitalWrite(i, (ex_temp[(idx>>3)] & (1 << (idx & 0x7))) != 0);
  }
}

inline void putNop() {
  unsigned char nop[4] = {0x13, 0, 0, 0};
  for(int i = 22; i < 54; i++) {
     int idx = i-22;
     digitalWrite(i, (nop[(idx>>3)] & (1 << (idx & 0x7))) != 0);
  }
}

int clock_pin = 2;
int handoff_pin = 3;
int dump_pin = 4;

void setupNop() {
    ex_temp[0x0] = 0x13;
    ex_temp[0x1] = 0x00;
    ex_temp[0x2] = 0x00;
    ex_temp[0x3] = 0x80;
    ex_temp[0x4] = 0x13;
    ex_temp[0x5] = 0x00;
    ex_temp[0x6] = 0x00;
    ex_temp[0x7] = 0x40;
}

void setupOutput() {
    in_handoff = false;
    digitalWrite(handoff_pin, HIGH);

    dumping = false;
    digitalWrite(dump_pin, HIGH);

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
  pinMode(dump_pin, OUTPUT);
  digitalWrite(dump_pin, HIGH);

  pinMode(handoff_pin, OUTPUT);
  setupOutput();

  setupNop();
  putTemp();

  pinMode(clock_pin, INPUT);  
  attachInterrupt(digitalPinToInterrupt(clock_pin), clk, CHANGE);

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200, SERIAL_8O1);
}

/*
Serial example

> P - ping
< P - pong

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

> D - dump data
> [4b len]
<< [4x len bytes]

*/

// W = wait
// Y = yield execution
char mode = 'W';

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
    case 'D': // dump
        dumping = false; // sanity
        run_temp = false;

        // get len to dump
        to_dump = 0;

        while(Serial.available() == 0);
        to_dump += Serial.read();
        to_dump <<= 8;
        while(Serial.available() == 0);
        to_dump += Serial.read();
        to_dump <<= 8;
        while(Serial.available() == 0);
        to_dump += Serial.read();
        to_dump <<= 8;
        while(Serial.available() == 0);
        to_dump += Serial.read();

        // send jalr zero, 0x0(x0); 00000067

        // wait for low clock
        while(digitalRead(clock_pin));

        ex_temp[0x0] = 0x67;
        ex_temp[0x1] = 0x00;
        ex_temp[0x2] = 0x00;
        ex_temp[0x3] = 0x00;

        start_dumping = true;
        toggle_handoff = true;
        run_temp = true;

        while(start_dumping || dumping) {
            while(!dump_next && !dump_done);
            dump_next = false;

            unsigned char buf[4] = {0, 0, 0, 0};
            for(int i = 22; i < 54; i++) {
               int idx = i-22;
               buf[(idx>>3)] |= (digitalRead(i) << (idx & 0x7));
            }

            Serial.write(buf[0]);
            Serial.write(buf[1]);
            Serial.write(buf[2]);
            Serial.write(buf[3]);

            if(dump_done) {
                break;
            }

        }
    break;
    default:
      Serial.write('?');
    }
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
        if(dumping && !start_dumping) {
            digitalWrite(dump_pin, LOW);
        }

        if(start_dumping) {
            start_dumping = false;
            dumping = true;
        }

        if(dumping) {
            if(to_dump <= 0) {
                dump_done = true;
                return;
            }
            to_dump--;
            dump_next = true;
        }

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

  putNop();
  digitalWrite(LED_BUILTIN, 0);
}

