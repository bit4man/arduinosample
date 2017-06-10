
// Address Setup
//Pin connected to ST_CP of 74HC595
#define latchPin 12
//Pin connected to SH_CP of 74HC595
#define clockPin 11
////Pin connected to DS of 74HC595
#define dataPin 10
#define clearAddressPin 13

  /* Byte configuration
   *  D9 D8 D7 D6 D5 D4 D3 D2
   *  MSB                     LSB
   */


/* 
 *  We need the following OPS
 *  READ  0x01
 *  ERASE 0x02
 *  WRITE 0x04
 *  
 */
#define OP_READ 1
#define OP_WRITE 2
#define OP_ERASE 4

byte MODE=OP_READ;

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clearAddressPin, OUTPUT);

  MODE=OP_READ;

  setDataRead();
    
  Serial.begin(115200);
}

void setDataRead() {
  for (int i=2; i<=9; i++) {
    pinMode(i, INPUT);
  }
}

void setDataWrite() {
  for (int i=2; i<=9; i++) {
    pinMode(i, OUTPUT);
  }
}

void setAddress(unsigned int address, byte mode) {
    setMode(mode);
    digitalWrite(clearAddressPin, LOW); // delay(1);
    digitalWrite(clearAddressPin, HIGH); delay(1); // Reset is inverted, HIGH means no reset
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, address);
    shiftOut(dataPin, clockPin, MSBFIRST, mode);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);  
}

void setMode(byte mode) {
  if (mode == MODE) return; // No need to reset it. 

  if (mode == OP_READ) {
    setDataRead();
  } else
  if (mode == OP_WRITE) {
    setDataWrite();
  }
  MODE=mode;
}

byte getData(unsigned int address) {
  setAddress(address, OP_READ);
  byte value = 0;
  for (int i=9; i>=2; i--) {
    value = ( value << 1 ) | digitalRead(i);
  }
  return value;
}

void writeData(unsigned int address, byte data) {
  setAddress(address, OP_WRITE);
  char buf[20];
  sprintf(buf,"Writing byte %02X\n", data);
  Serial.write(buf);
  for (int i=9; i>=2; i--) {
    byte mask = 1 << ( i - 2 );
    if ( (data&mask) > 0) 
      digitalWrite(i, HIGH);
     else
      digitalWrite(i, LOW);
  }
}

void dumpData(unsigned int fromAddr, unsigned int toAddr) {
  const int NUMDATA=16;
//  Serial.begin(9600);

  Serial.println("\nDump Data");
  
  if (toAddr < fromAddr) return;
  if ((fromAddr < 0) || (toAddr < 0)) return;
  if ((fromAddr > 0xFFFF) || (toAddr > 0xFFFF)) return;

  char addrStr[] = "\n%04X: ";
  char hexStr[] = "%02X ";
  int addr=fromAddr;
  int newLine=0;
  while (addr <= toAddr) {
    if (addr >= newLine) { // change lines
      char buf[20];
      sprintf(buf, addrStr, addr);
      Serial.print(buf);
      newLine = addr + NUMDATA;
    }
    char buf[20];
    sprintf(buf, hexStr, getData(addr));
    Serial.print(buf);
    addr++;
  }
//  Serial.end();
}

byte datatest = 0;
int steps = 0;
const int activeTicks = 10;

void loop() {
  if (steps++ > activeTicks) {
    steps = 0;
    writeData(0x11, datatest++);
  }
  
  if (Serial.available() > 0) {
     // read the incoming byte:
     char ch = Serial.read();

     if (ch == 'd')
        dumpData(0,100);
  }
  delay(10);
}

