/* 
 *  EEPROM reader/writer
 *  AT28C64B
 *  
 *  D2 = Address Data bus
 *  D3 = Serial Clock 
 *  D4 = Storage Enable
 *  OE = A?
 *  WE = A?
 *  CE = A?
 *  
 *  Data Pins:
 *  D5 : LSB -> D12 MSB (8 bits)
 */

#define ADDR_DATA 2
#define ADDR_CLK 3
#define ADDR_LATCH 4
#define ADDR_CLEAR 13
#define WE A2
#define OE A1
#define CE A0

#define DATA_START 5
#define DATA_END 12

void setOutputEnabled(bool enabled) {
  if (enabled) digitalWrite(OE, LOW);
  else digitalWrite(OE, HIGH);
}

void setAddress(unsigned int address) {
    digitalWrite(ADDR_CLEAR, LOW);  // delay(1);
    digitalWrite(ADDR_CLEAR, HIGH); // delay(1); // Reset is inverted, HIGH means no reset
    digitalWrite(ADDR_LATCH, LOW);
    // shift out the bits:
    shiftOut(ADDR_DATA, ADDR_CLK, MSBFIRST, (address >> 8));
    shiftOut(ADDR_DATA, ADDR_CLK, MSBFIRST, (byte) address);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(ADDR_LATCH, HIGH);
}

#define OP_READ 1
#define OP_WRITE 2
byte MODE=0;

void setDataRead() {
  if (MODE == OP_READ) return;
  MODE=OP_READ;
//  Serial.println("Set Read State");
  for (int i=DATA_START; i<=DATA_END; i++) {
    pinMode(i, INPUT);
  delay(1); // give time tos et
  }
}

void setDataWrite() {
  if (MODE == OP_WRITE) return;
  MODE=OP_WRITE;
//  Serial.println("Set WRITE State");
  for (int i=DATA_START; i<=DATA_END; i++) {
    pinMode(i, OUTPUT);
  }
}

byte readBus() {
  byte value = 0;
  setDataRead();
  for (int i=DATA_START; i<=DATA_END; i++) {
    value = ( value << 1 ) | digitalRead(i);
  }
  return value;  
}

byte getData(unsigned int address) {
  setAddress(address);
  setDataRead();
  setOutputEnabled(true);
  digitalWrite(CE,LOW); // Disable to initiate read
//  delay(1);
  byte value = readBus();
  setOutputEnabled(false);
  digitalWrite(CE,HIGH); // Enable to stop read
  return value;
}

void writeData(unsigned int address, byte data) {
  setOutputEnabled(false);
  setAddress(address);
  setDataWrite();
  digitalWrite(CE, LOW); // Chip "disabled" for write
  char buf[40]; sprintf(buf,"(%04X) Writing byte %02X\n", address, data);
  Serial.write(buf);
  int shift=7;
  int bit=DATA_START;
  while (shift>=0)
  {
    byte mask = 1 << shift--;
    if ( (data&mask) > 0) {
      digitalWrite(bit, HIGH);
    } else {
      digitalWrite(bit, LOW);
    }
    bit++;
  }
  digitalWrite(WE, LOW); // begin write
  delay(5); // Hold for write to take // min wait 100 ns
  digitalWrite(WE, HIGH); // Done Writing
  digitalWrite(CE, HIGH); // Renable chip
// Verify Data
  byte check = readBus();

  int timeout=10; 
  while ((timeout > 0) && (data != check)) {
    if (data != (check ^ 0xFF)) { // Chip sets complementary result when done
      timeout--; 
    } 
    if ((timeout > -1) && (data != (check ^ 0xFF))) {
      char b[20];
      sprintf(b,"Write error: %02X/%02X\n",data,check);
      Serial.write(b);
    }
    check = readBus();
  }
  setOutputEnabled(true);
}

void dumpData(unsigned int fromAddr, unsigned int toAddr) {
  const int NUMDATA=16;
//  Serial.begin(9600);

  Serial.println("\nDump Data");
  setDataRead(); delay(10);
  
  if (toAddr < fromAddr) return;
  if ((fromAddr < 0) || (toAddr < 0)) return;
  if ((fromAddr > 0xFFFF) || (toAddr > 0xFFFF)) return;

  char addrStr[] = "\n%04X: ";
  char hexStr[] = "%02X ";
  int addr=fromAddr;
  int newLine=0;
  while (addr <= toAddr) {
    if (addr >= newLine) { // change lines
      char buf[10];
      sprintf(buf, addrStr, addr);
      Serial.print(buf);
      newLine = addr + NUMDATA;
    }
    char buf[10];
    byte data=getData(addr);
    sprintf(buf, hexStr, data);
    Serial.print(buf);
    addr++;
  }
//  Serial.end();
}

void testWrite() {
  int addr=0;
  for (int data=0; data<=0xFF; data++) {
    writeData(addr++, data);
  }
}

void setup() {
  pinMode(ADDR_DATA, OUTPUT);
  pinMode(ADDR_CLK, OUTPUT);
  pinMode(ADDR_LATCH, OUTPUT);
  pinMode(ADDR_CLEAR, OUTPUT);
  pinMode(WE, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(CE, OUTPUT);
  Serial.begin(115200);

  // Disable operations for now
  digitalWrite(CE, HIGH); // Disable chip by default
  digitalWrite(WE, HIGH);
  digitalWrite(OE, HIGH);
  
  dumpData(0, 255);
//  testWrite();
//  dumpData(0, 255);
}

void loop() {
  digitalWrite(CE, HIGH); // Stop doing things
  setOutputEnabled(false);
  // put your main code here, to run repeatedly:
  delay(500);
  char buf[50];
  int addr=random(0xFF);
  sprintf(buf,"Adderess 0x%04X is 0x%02X\n", addr, getData(addr));
  Serial.println(buf);
}
