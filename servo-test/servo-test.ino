#include <Servo.h> //servo library
Servo myservo; // create servo object to control servo

const int LEN=3;
int pos[LEN] = {20,90,190};


void setup() {
  // put your setup code here, to run once:
  myservo.attach(3);// attach servo on pin 3 to servo object
  Serial.begin(9600);
}

void loop() {
  int i;
  for (i=0; i < LEN; i++) {
    myservo.write(pos[i]);
    Serial.print("Pos: ");
    Serial.println(pos[i]);
    delay(2500);
  }
  // put your main code here, to run repeatedly:

}
