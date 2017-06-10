#include <TimerOne.h>

int pwm_a = 3;  //PWM control for motor outputs 1 and 2 
int pwm_b = 9;  //PWM control for motor outputs 3 and 4 
int dir_a = 7;  //direction control for motor outputs 1 and 2 
int dir_b = 8;  //direction control for motor outputs 3 and 4 

unsigned int counter=0;
 
void docount()  // counts from the speed sensor
{
  counter++;  // increase +1 the counter value
} 
 
void timerIsr()
{
  Timer1.detachInterrupt();  //stop the timer
  Serial.print("Motor Speed: "); 
  int rotation = (counter / 1);  // divide by number of holes in Disc
  Serial.print(rotation,DEC);  
  Serial.println(" Rotation per seconds"); 
  counter=0;  //  reset counter to zero
  Timer1.attachInterrupt( timerIsr );  //enable the timer
}

void setup()
{
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  
  analogWrite(pwm_a, 255);  // Start A full speed
  Serial.begin(9600);

  Timer1.initialize(1000000); // set timer for 1sec
  attachInterrupt(0, docount, RISING);  // increase counter when speed sensor pin goes High
  Timer1.attachInterrupt( timerIsr ); // enable the timer
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(pwm_a, 255);  // Start A full speed
  digitalWrite(dir_a, LOW); 
  delay(200);
  for (int i=255; i>150;i=i-10) {
    Serial.print("Speed: ");
    Serial.println(i,DEC);
    analogWrite(pwm_a, i);
    delay(2000);
  }
}
