uint8_t pin_cl_latch = 1 << 4;
uint8_t pin_cl_show = 1 << 5;
uint8_t pin_reg_w = 1 << 6;

uint8_t ppin_cl_latch = 8;
uint8_t ppin_cl_show = 9;
uint8_t ppin_reg_w = 10;

int pin_mem_act = A2;

int pin_freqmod = A5;

int pin_single = 2;
int pin_mode = 3;

int pin_trap_enable = 13;
int pin_trap_level = 11;
int pin_trap = 12;

void setup() {
  pinMode(ppin_cl_latch, OUTPUT);
  pinMode(ppin_cl_show, OUTPUT);
  pinMode(ppin_reg_w, OUTPUT);

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

uint8_t i = latch_hi;

inline void writeb(uint8_t bit, uint8_t val) {
    if (val == LOW) {
        PORTB &= ~bit;
    } else {
        PORTB |= bit;
    }
}

void update() {
    switch (i) {
    case latch_hi:
        writeb(pin_cl_latch, HIGH);
        writeb(pin_reg_w, LOW);
    break;
    case latch_lo:
        writeb(pin_cl_latch, LOW);
        writeb(pin_reg_w, LOW);
    break;
    case mem_hi:
        writeb(pin_cl_latch, LOW);
        writeb(pin_reg_w, HIGH);
    break;
    case mem_lo:
        writeb(pin_cl_latch, LOW);
        writeb(pin_reg_w, LOW);
    break;
    case show_hi:
        writeb(pin_cl_show, HIGH);
        writeb(pin_reg_w, LOW);
    break;
    case show_lo:
        writeb(pin_cl_show, LOW);
        writeb(pin_reg_w, LOW);
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
    int32_t d = analogRead(pin_freqmod)-20;
    if(d <= 0) d = 1;

    int32_t dus = d*d;

    uint8_t j = 1;
    if(dus == 1) {
        for(uint8_t k = 0; k < 200; k++) {
            for(; i <= show_lo; i++) {
                update();
            }
            if(i > show_lo) {
              i = latch_hi;
            }
        }
    } else {
        for(; i <= show_lo; i++) {
            if(dus > 1024) {
                delay(dus/1024);
            } else {
                delayMicroseconds(dus);
            }

            if(digitalRead(pin_trap_enable)==0 && digitalRead(pin_trap_level) == digitalRead(pin_trap)) {
                mode_single = true;
                return;
            }

            update();
        }
    }
  }

  if(i > show_lo) {
    i = latch_hi;
  }
}
