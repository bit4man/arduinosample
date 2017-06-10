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
int clearPin=7;
int outputEnablePin=6;

void clear() {
   digitalWrite(outputEnablePin, HIGH);
   digitalWrite(clearPin, LOW);
   delay(50);
   digitalWrite(clearPin, HIGH);
   digitalWrite(outputEnablePin, LOW);
}

void setup() {
  // Setup pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clearPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT);
  clear();
  Serial.begin(9600);
}

void setLeds(byte leds) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}


void loop() {
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

  clear();  

  for (byte seq=0; seq<5; seq++) {
    setLeds(meet_led[seq]);
    delay(200);
  };
  clear();  

  for (byte seq=4;(seq<=4); seq--) {
    sprintf(buff,"%02d",seq);
    Serial.write(buff);
    Serial.write("\n");
    setLeds(meet_led[seq]);
    delay(200);
  }
  clear();  


}
