int pin_cut = 23;
int pin_mes_fast = 25;
int pin_mes_slow = 27;
int pin_retr = 29;
int pin_rstc = 31;

int pin_next = 33;

int pin_disp_0 = 35;
int disp_pins = 7;

int pin_enable = 38;

int d_fast = 400;
int d_mid = 900;
int d_slow = 4000;

int dir_fwd = HIGH;
int dir_back = LOW;

void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(pin_enable, OUTPUT);
  digitalWrite(pin_enable, LOW);

  pinMode(pin_cut, INPUT_PULLUP);
  pinMode(pin_mes_fast, INPUT_PULLUP);
  pinMode(pin_mes_slow, INPUT_PULLUP);
  pinMode(pin_retr, INPUT_PULLUP);
  pinMode(pin_rstc, INPUT_PULLUP);
  pinMode(pin_next, INPUT_PULLUP);

  for(int i = 0; i < disp_pins; i++){
    pinMode(pin_disp_0 + (i * 2), OUTPUT);
    digitalWrite(pin_disp_0 + (i * 2), HIGH);
  }
}

int32_t len = 0;

int32_t cycleUntilPin(int pin, int dir, int speed) {
  digitalWrite(A1, dir);

  int debounce = 10;
  int32_t n = 0;

  while(1) {
    if(digitalRead(pin)) {
      debounce--;
      if(debounce == 0) {
        return n;
      }
      delayMicroseconds(speed);
      continue;
    }

    debounce = 10;

    n++;
    
    digitalWrite(A0, HIGH);
    delayMicroseconds(speed/4);
    digitalWrite(A0, LOW);
    delayMicroseconds(speed);  
  }

  return 0;  
}    

void next(int32_t len) {
  digitalWrite(A1, dir_fwd);

  for(int32_t i = 0; i < len; i++) {
    digitalWrite(A0, HIGH);
    delayMicroseconds(d_fast/4);
    digitalWrite(A0, LOW);
    delayMicroseconds(d_fast);  
  }
}

int cut = 0;

void loop() {  
  delay(50); // debounce

  for(int i = 0; i < disp_pins; i++){
    digitalWrite(pin_disp_0 + (i * 2), (cut & (1 << i)) > 0);
  }

  if(!digitalRead(pin_rstc)){
    cut = 0;
  }
  if(!digitalRead(pin_cut)){
    len = 0;
  }
  if(!digitalRead(pin_mes_slow)){
    len += cycleUntilPin(pin_mes_slow, dir_fwd, d_slow);
    return;
  }
  if(!digitalRead(pin_mes_fast)){
    len += cycleUntilPin(pin_mes_fast, dir_fwd, d_mid);
    return;
  }
  if(!digitalRead(pin_retr)){
    len -= cycleUntilPin(pin_retr, dir_back, d_slow);
    if(len < 0) {
      len = 0;      
    }
    return;
  }
  if(!digitalRead(pin_next)){
    next(len);
    cut++;

    int debounce = 20;
    while(debounce != 0) {
      delay(4);
      if(digitalRead(pin_next)) {
        debounce--;
      } else {
        debounce = 20;
      }
    }  
    return;
  }
  

  
  /*while(digitalRead(23));

  digitalWrite(A1, LOW);
  
  int d = d_fast;
  
  for(int i = 0; i < 50; i++) {      
    digitalWrite(A0, HIGH);
    delayMicroseconds(d/4);
    digitalWrite(A0, LOW);
    delayMicroseconds(d);  
  }

  digitalWrite(A1, HIGH);

  for(int i = 0; i < 50; i++) {      
    digitalWrite(A0, HIGH);
    delayMicroseconds(d/4);
    digitalWrite(A0, LOW);
    delayMicroseconds(d);  
  }
  */
}
