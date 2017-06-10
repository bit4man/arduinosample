const char fmt[] = "Value (%02X,%02X): %02X\n";

void setup() {
  // Set pins as input (0)
  DDRD = DDRD & B00111111; // D2-D7 input
  DDRB = B00000000;        // D8 and D9 
  Serial.begin(115200);
//  Serial.write(PINB);
//  Serial.write("\n\n");
}

byte oldValue = 0;

void loop() {
  // put your main code here, to run repeatedly:
  byte dataLow = PINB; // low 6 bits
  byte dataHigh = PIND; // low 2 bits are the 2 high bits
  byte data = (dataLow & B00111111) | (( dataHigh & B11000000 ) );

  if (oldValue != data) {
    char buf[20]; sprintf(buf,fmt,dataHigh,dataLow,data);
    Serial.print(buf);
    oldValue = data;
  }
  delay(200);
}
