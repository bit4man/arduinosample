const int DS_pin   = 8;
const int STCP_pin = 9;
const int SHCP_pin = 10;
#define DIG1SEL_pin 7
#define DIG2SEL_pin 6
#define DIG3SEL_pin 5
#define DIG4SEL_pin 4


/*
 * G = 1
 * F = 2
 * A = 4
 * B = 8
 * E = 16
 * D = 32
 * C = 64
 * DP = 128
 * 
 *   AAAA
 *   F  B
 *   F  B
 *    GG
 *   E  C
 *   E  C
 *   DDDD DP
 *   
 *   0 = A+F+B+E+C+D
 *   1 = B+C
 *   2 = A+B+G+E+D
 *   3 = A+B+G+C+D
 *   4 = F+B+G+C
 *   5 = A+F+G+C+D
 *   6 = F+E+G+C+D
 *   7 = A+B+C
 *   8 = A+B+C+D+E+F+G
 *   9 - A+B+C+D+F+G
 *   A = A+B+F+G+E+C
 *   B = F+E+D+C+G
 *   C = A+F+E+D
 *   D = B+C+G+D+E
 *   E = A+F+G+E+D
 *   F = A+F+G+E
 */

 const byte dA = 0x04;
 const byte dB = 0x08;
 const byte dC = 0x40;
 const byte dD = 0x20;
 const byte dE = 0x10;
 const byte dF = 0x02;
 const byte dG = 0x01;
 const byte dDP = 0x80;

bool digit1=true;
unsigned long counter=0;
byte thousands, hundreds, tens, ones;

 const byte numbers[] = {
     /*0*/ dA | dF | dB | dE | dC | dD,
     /*1*/ dB | dC,
     /*2*/ dA | dB | dG | dE | dD,
     /*3*/ dA | dB | dG | dC | dD,
     /*4*/ dF | dB | dG | dC,
     /*5*/ dA | dF | dG | dC | dD,
     /*6*/ dF | dE | dG | dC | dD,
     /*7*/ dA | dB | dC,
     /*8*/ dA | dB | dC | dD | dE | dF | dG,
     /*9*/ dA | dB | dC | dD | dF | dG,
     /*A*/ dA | dB | dF | dG | dE | dC, 
     /*B*/ dF | dE | dD | dC | dG,
     /*C*/ dA | dF | dE | dD,
     /*D*/ dB | dC | dG | dD | dE,
     /*E*/ dA | dF | dG | dE | dD,
     /*F*/ dA | dF | dG | dE 
 };



void writeDigits(byte pattern) {
  digitalWrite(STCP_pin,LOW);
  shiftOut(DS_pin,SHCP_pin,MSBFIRST,pattern);
  digitalWrite(STCP_pin, HIGH);
}

void test() {
  byte p = 1;
  while (p > 0) {
    writeDigits(p);
    p = p << 1;
    delay(100);
  }

}

void setDigit(byte digit) {
  digitalWrite(DIG1SEL_pin,HIGH);
  digitalWrite(DIG2SEL_pin,HIGH);
  digitalWrite(DIG3SEL_pin,HIGH);
  digitalWrite(DIG4SEL_pin,HIGH);

  int pin = 0;
  
  switch (digit) {
     case 1: pin=DIG1SEL_pin; break;
     case 2: pin=DIG2SEL_pin; break;
     case 3: pin=DIG3SEL_pin; break;
     case 4: pin=DIG4SEL_pin; break;
  }
  if (pin>0) digitalWrite(pin,LOW);
}

void debugNum(int n, long num) {
  char buf[100];
  sprintf(buf," (%d):%0d ", n, num);
  Serial.print(buf);
}

void dispNum(long num) {
  long n = num;
  
  while (n > 9999) n = n-1000;
  Serial.println(n);
  thousands = n / 1000;
  n = n-(thousands*1000);
  hundreds  = n / 100;
  n = n-(hundreds*100);
  tens = n / 10;
  n = n-(tens*10);
  ones = n;
  if (thousands >9) thousands=0;
  if (hundreds  >9) hundreds=0;
  if (tens      >9) tens=0;

  char buf[100];
  sprintf(buf,"%d %d %d %d\n",thousands,hundreds,tens,ones);

  Serial.write(buf);
  
}

void setup() {
  // put your setup code here, to run once:
  pinMode(DS_pin,OUTPUT);
  pinMode(STCP_pin,OUTPUT);
  pinMode(SHCP_pin,OUTPUT);
  pinMode(DIG1SEL_pin,OUTPUT);
  pinMode(DIG2SEL_pin,OUTPUT);
  pinMode(DIG3SEL_pin,OUTPUT);
  pinMode(DIG4SEL_pin,OUTPUT);
  
  
  Serial.begin(9600);
  Serial.println("BEGIN");
  
   for (int i=0; i < sizeof(numbers); i++) {
     char fmt[] = "%d: %02X\n";
     char str[40];
    sprintf(str,fmt,i,numbers[i]);
    Serial.print(str);
   }

   dispNum(0);
}

int inner=0;

void loop() {

  if (inner > 50) {
    inner = 0;
    dispNum(counter++);
    if (counter>9999) counter=0;
  } else inner++;
     
  // Transfer to display
  setDigit(1);
  writeDigits(numbers[thousands]);
  delay(5);
  setDigit(2);
  writeDigits(numbers[hundreds]);
  delay(5);
  setDigit(3);
  writeDigits(numbers[tens]);
  delay(5);
  setDigit(4);
  writeDigits(numbers[ones]);
  delay(5); 
}
