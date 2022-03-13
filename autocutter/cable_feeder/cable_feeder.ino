#include <LiquidCrystal.h>

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

LiquidCrystal lcd(pin_disp_0, pin_disp_0+2, pin_disp_0+4, pin_disp_0+6, pin_disp_0+8, pin_disp_0+10);

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

  lcd.begin(16, 1);
  lcd.print("init111122223333");
}

int32_t len = 0;
int32_t seq_min = 0;
int32_t seq_max = 0;

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
char text[64];

#define M_simple 'S'

#define M_seq_cnt 'c'
#define M_seq_at 'a'
#define M_seq_min 'm'
#define M_seq_max 'M'
#define M_seq 'q'

int seq_count = 0;
int seq_at = 0;

char mode = M_simple;

void handle_len() {
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
}


void loop() {
  while(!digitalRead(pin_rstc)) {
    delay(50);
  }
  delay(50); // debounce
  
  switch(mode){
    case M_simple:
      simple();
      break;
    case M_seq_cnt:
      seq_cnt();
      break;
    case M_seq_at:
      seq_at_set();
      break;
    case M_seq_min:
      seq_min_set();
      break;
    case M_seq_max:
      seq_max_set();
      break;
    case M_seq:
      seq();
      break;            
  }
}  


void seq_cnt() {
  lcd.setCursor(0, 0);
  sprintf(text, "s Cnt %d          ", seq_count);
  text[16] = 0;
  lcd.write(text);

  if(!digitalRead(pin_rstc)){
    mode = M_seq_at;
    return;
  }

  if(!digitalRead(pin_retr)){
    seq_count = 0;
  }
  if(!digitalRead(pin_mes_fast)){
    seq_count++;
  }
  if(!digitalRead(pin_mes_slow)){
    seq_count--;
  }
}

void seq_at_set() {
  lcd.setCursor(0, 0);
  sprintf(text, "s @ %d          ", seq_at);
  text[16] = 0;
  lcd.write(text);

  if(!digitalRead(pin_rstc)){
    mode = M_seq_min;
    return;
  }

  if(!digitalRead(pin_retr)){
    seq_at = 0;
  }
  if(!digitalRead(pin_mes_fast)){
    seq_at++;
  }
  if(!digitalRead(pin_mes_slow)){
    seq_at--;
  }
}

void seq_min_set() {
  lcd.setCursor(0, 0);
  snprintf(text, 64, "sMin %ld<%ld          ", seq_min, len);
  text[16] = 0;
  lcd.write(text);

  if(!digitalRead(pin_rstc)){
    mode = M_seq_max;
    return;
  }

  handle_len();

  if(!digitalRead(pin_next)){
    seq_min = len;
  }
}

void seq_max_set() {
  lcd.setCursor(0, 0);
  sprintf(text, "sMax %ld<%ld          ", seq_max, len);
  text[16] = 0;
  lcd.write(text);

  if(!digitalRead(pin_rstc)){
    mode = M_seq;
    return;
  }

  handle_len();

  if(!digitalRead(pin_next)){
    seq_max = len;
  }
}

void seq() {
  if(!digitalRead(pin_rstc)){
    mode = M_simple;
    return;
  }
  
  lcd.setCursor(0, 0);

  if(seq_max < 1) {
    lcd.write("s! Max <= 0");
    return;
  }
  if(seq_min < 1) {
    lcd.write("s! Min <= 0");
    return;
  }
  if(seq_count < 1) {
    lcd.write("s! Count <= 0");
    return;
  }
  if(seq_at >= seq_count) {
    lcd.write("s! At >= Count");
    return;
  }
  if(seq_min > seq_max) {
    lcd.write("s! Min > Max");
    return;
  }

  int32_t increment = (seq_max-seq_min)/seq_count;
  int32_t cur_len = seq_min+(increment*seq_at);

  sprintf(text, "s@%d %ld+%ld         ", seq_at, cur_len, increment);
  text[16] = 0;
  lcd.write(text);

  if(!digitalRead(pin_next)){
    next(cur_len);
    cut++;
    seq_at++;

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
}

void simple() {
  lcd.setCursor(0, 0);
  sprintf(text, "len %ld       ", len);
  text[16] = 0;
  lcd.write(text);

  for(int i = 0; i < disp_pins; i++){
    digitalWrite(pin_disp_0 + (i * 2), (cut & (1 << i)) > 0);
  }

  if(!digitalRead(pin_rstc)){
    mode = M_seq_cnt;
    return;
  }
  
  if(!digitalRead(pin_cut)){
    len = 0;
  }
  handle_len();
  
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
}
