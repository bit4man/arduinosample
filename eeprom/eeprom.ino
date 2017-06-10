
// Address Setup
//Pin connected to ST_CP of 74HC595
#define latchPin 12
//Pin connected to SH_CP of 74HC595
#define clockPin 11
////Pin connected to DS of 74HC595
#define dataPin 10
#define clearAddressPin 13

  /* Byte configuration
   *  D7 D6 D13 D12 D11 D10 D9 D8
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

  // Data
  // Set pins as input (0)
  DDRD = DDRD & B00111111; // D2-D7 data in/out
  DDRB = B00000000;        // D8 and D9 
  
  Serial.begin(115200);
}

void setAddress(unsigned int address, byte mode) {
    setMode(mode);
    digitalWrite(clearAddressPin, HIGH); // delay(1);
    digitalWrite(clearAddressPin, LOW); delay(1);
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
    // Set pins as input (0)
    DDRB = B00000000;        // D8 and D9 
    DDRD = DDRD & B00111111; // D2-D7 input
  } else
  if (mode == OP_WRITE) {
    // Set pins as output (0)
    DDRB = DDRB | B00000011;        // D8 and D9     
    DDRD = DDRD | B11111100; // D2-D7 output
  }
  MODE=mode;
}

byte getData(unsigned int address) {
  setAddress(address, OP_READ);
  byte dataLow = PINB; // low 6 bits
  byte dataHigh = PIND; // low 2 bits are the 2 high bits
  return (byte) (dataLow & B00111111) | (( dataHigh & B11000000 ) );
}

void writeData(unsigned int address, byte data) {
  setAddress(address, OP_WRITE);
  char buf[20];
  sprintf(buf,"Writing byte %02X\n", data);
  Serial.write(buf);
  PORTD = ( PORTD & B11000000 ) | ( ( data & B00111111 ) << 2);
  PORTB = ( PORTB & B11111100 ) | ( ( data & B11000000 ) >> 6);
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

