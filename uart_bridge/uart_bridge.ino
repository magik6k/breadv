
// PORT L: RXB
// PORT C: TXB

const uint8_t TX_BUSY = 22;
const uint8_t TX_DONE = 23;

const uint8_t RX_BUSY = 24;
const uint8_t RX_RECVD = 25;

void setup() {
    Serial.begin(115200);

    // Set up the pins for the UART
    DDRL = 0xff; // RXB
    PORTL = 0xaa;

    DDRC = 0x00; // TXB
    PORTC = 0x00;

    pinMode(TX_BUSY, INPUT);
    pinMode(TX_DONE, OUTPUT);

    pinMode(RX_RECVD, OUTPUT);
    pinMode(RX_BUSY, INPUT);

    digitalWrite(RX_RECVD, HIGH); // nand rs latches
    digitalWrite(TX_DONE, HIGH);
}

void loop() {
    if(digitalRead(TX_BUSY)) {
        // TX_BUSY means that there is a byte to transfer, see if we can do that
        if(Serial.availableForWrite() > 0) {
            // We can write a byte, so do that
            Serial.write(PINC);
            // And tell the other side that we're done
            digitalWrite(TX_DONE, LOW);
            digitalWrite(TX_DONE, HIGH);
        }
    }

    if(!digitalRead(RX_BUSY)) {
        // zero RX_BUSY means that we can receive a byte, see if we can do that
        if(Serial.available() > 0) {
            // We can read a byte, so do that
            PORTL = Serial.read();
            // And tell the other side that byte was received
            digitalWrite(RX_RECVD, LOW); // todo check timing
            digitalWrite(RX_RECVD, HIGH);
        }
    }
}