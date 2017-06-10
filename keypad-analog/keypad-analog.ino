#define VIN A2


void setup() {
  pinMode(VIN, INPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
}

int last = 0;
const int DIV=8;

void loop() {
  int val=analogRead(VIN);

  int divMin=last-DIV;
  int divMax=last+DIV;
  
  if ((val < divMin) || (val > divMax)) {
      Serial.println(val);
      last = val;
    }
  delayMicroseconds(20);

}
