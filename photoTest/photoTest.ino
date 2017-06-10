//www.RobotLinking.com
//2015.5.7
/*
  Bargraph sketch

 Turns on a series of LEDs proportional to a value of an analog sensor.
 Six LEDs are controlled but you can change the number of LEDs by changing
 the value of NbrLEDs and adding the pins to the ledPins array
 */

const int NbrLEDs = 8;
const int ledPins[] = { 5, 6, 7, 8, 9, 10, 11, 12};
const int photocellPin = A0;                          
int sensorValue = 0;        // value read from the sensor
int ledLevel = 0;           // sensor value converted into LED 'bars'

void setup() {
  for (int led = 0; led < NbrLEDs; led++)
  {
    pinMode(ledPins[led], OUTPUT);// make all the LED pins outputs
  }
  Serial.begin(9600);
}

void loop() {
  
  sensorValue = analogRead(photocellPin);
  Serial.print(sensorValue);
  ledLevel = map(sensorValue, 300, 1000, 0, NbrLEDs);  // map to the number of LEDs
  Serial.print(" level:");
  Serial.print(ledLevel);
  for (int led = 0; led < NbrLEDs; led++)
  {
    if (led < ledLevel ) {
      digitalWrite(ledPins[led], HIGH);     // turn on pins less than the level
    }
    else {
      digitalWrite(ledPins[led],LOW);      // turn off pins higher than 
                                                // the level
    }
  }
  Serial.println();
}
      
    
