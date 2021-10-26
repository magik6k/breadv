int pin_cl_latch = A0;
int pin_cl_show = A1;
int pin_reg_w = 4;

int pin_cl_latch_mem = A3;
int pin_cl_show_mem = A4;

int pin_mem_act = A2;

int pin_freqmod = A5;

int pin_single = 2;
int pin_mode = 3;

int pin_trap_enable = 10;
int pin_trap_level = 11;
int pin_trap = 12;

void setup() {
  pinMode(pin_cl_latch, OUTPUT);
  pinMode(pin_cl_show, OUTPUT);
  pinMode(pin_reg_w, OUTPUT);

  pinMode(pin_trap_enable, INPUT_PULLUP);
  pinMode(pin_trap_level, INPUT_PULLUP);
  pinMode(pin_trap, INPUT);

  pinMode(pin_single, INPUT_PULLUP);
  pinMode(pin_mode, INPUT_PULLUP);
}

enum state {  
  latch_hi,
  latch_lo,

  mem_hi,
  mem_lo,
    
  show_hi,
  show_lo,
};

bool mode_single = true;

int i = latch_hi;

void update() {
    switch (i) {
    case latch_hi:
        digitalWrite(pin_cl_latch, HIGH);
        digitalWrite(pin_reg_w, LOW);
    break;
    case latch_lo:
        digitalWrite(pin_cl_latch, LOW);
        digitalWrite(pin_reg_w, LOW);
    break;
    case mem_hi:
        digitalWrite(pin_cl_latch, LOW);
        digitalWrite(pin_reg_w, HIGH);
    break;
    case mem_lo:
        digitalWrite(pin_cl_latch, LOW);
        digitalWrite(pin_reg_w, LOW);
    break;
    case show_hi:
        digitalWrite(pin_cl_show, HIGH);
        digitalWrite(pin_reg_w, LOW);
    break;
    case show_lo:
        digitalWrite(pin_cl_show, LOW);
        digitalWrite(pin_reg_w, LOW);
    break;
    }
}

void loop() {
  if(digitalRead(pin_mode)==0){
    mode_single = !mode_single;

    for(int db = 0; db < 10; db++) {
        delay(4);
        if(!digitalRead(pin_mode)) {
            db = 0;
        }
    }
  }

  if(mode_single) {
    delay(50);

    update();

    i++;

    while(digitalRead(pin_mode) && digitalRead(pin_single));
    
    if(!digitalRead(pin_single)) {
      for(int db = 0; db < 10; db++) {
          delay(4);
          if(!digitalRead(pin_single)) {
              db = 0;
          }
      }
  }

  } else {
    int d = analogRead(pin_freqmod)+1;

    for(; i <= show_lo; i++) {
        delay(d);

        if(digitalRead(pin_trap_enable)==0 && digitalRead(pin_trap_level) == digitalRead(pin_trap)) {
            mode_single = true;
            return;
        }

        update();
    }
  }

  if(i > show_lo) {
    i = latch_hi;
  }
}
