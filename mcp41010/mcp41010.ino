/*MCP41010 Tutorial*/

#include <SPI.h>  
const int CS = 10;
int PotWiperVoltage = 1;
int RawVoltage = 0;
float Voltage = 0;
 
void setup() {
  pinMode (CS, OUTPUT);   
  Serial.begin(9600);
  SPI.begin();    
}
 
void loop() {
  // move the potentiometer in one direction
  for (int level = 0; level < 255; level++) 
  {
    MCP41010Write(level);
    delay(100);
    RawVoltage = analogRead(PotWiperVoltage);
    Voltage = (RawVoltage * 5.0 )/ 1024.0;
    Serial.print("Level = " );                      
    Serial.print(level);      
    Serial.print("\t Voltage = ");
    Serial.println(Voltage,3);  
  }
  delay(2000);  // wait a couple seconds
  // Now mover potentiometer in other directions
  
  for (int level = 255; level > 0; level--) 
  {
    MCP41010Write(level);
    delay(100);
    RawVoltage = analogRead(PotWiperVoltage);
    Voltage = (RawVoltage * 5.0 )/ 1024.0;
    Serial.print("Level = " );                      
    Serial.print(level);      
    Serial.print("\t Voltage = ");
    Serial.println(Voltage,3); 
  }
   delay(2000);
}
 
void MCP41010Write(byte value) 
{
  // Note that the integer vale passed to this subroutine
  // is cast to a byte
  
  digitalWrite(CS,LOW);
  SPI.transfer(B00010001); // This tells the chip to set the pot
  SPI.transfer(value);     // This tells it the pot position
  digitalWrite(CS,HIGH); 
}


