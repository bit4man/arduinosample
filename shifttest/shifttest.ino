byte scroll_led[8] = { 
  B10000000, 
  B01000000,
  B00100000,
  B00010000,
  B00001000,
  B00000100,
  B00000010,
  B00000001
};

byte meet_led[5] = { 
  B00000000, 
  B10000001,
  B01000010,
  B00100100,
  B00011000
};

int latchPin=8;
int clockPin=12;
int dataPin=11;

void setup() {
  // Setup pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
}

void setLeds(byte leds) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}

void old() {
  char buff[10];
  for (byte seq=0; seq<8; seq++) {
    Serial.write("Seq:");
    sprintf(buff,"%02d",seq);
    Serial.write(buff);
//    Serial.write(" is ");
//    Serial.write(scroll_led[seq]);
    Serial.write("\n");
    setLeds(scroll_led[seq]);
    delay(200);
  };

  for (byte seq=0; seq<5; seq++) {
    setLeds(meet_led[seq]);
    delay(200);
  };
  for (byte seq=4;(seq<=4); seq--) {
    sprintf(buff,"%02d",seq);
    Serial.write(buff);
    Serial.write("\n");
    setLeds(meet_led[seq]);
    delay(200);
  }
  for (int i=1; (i<25); i++) {
    setLeds((byte) random(255));
    delay(100);
  }  
}

byte disp = 0;

void loop() {
  int LOOPS=50;

  // Forward Scroll
  for (int i=0; i<LOOPS; i++) {
    disp <<= 1;
    if (random(11) > 5) {
      disp |= 1;
    }
    setLeds(disp);
    delay(150);
  }

  //Backward Scroll
  for (int i=0; i<LOOPS; i++) {
    disp >>= 1;
    if (random(11) > 5) {
      disp |= 128;
    }
    setLeds(disp);
    delay(150);
  }


}
