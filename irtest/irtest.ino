#include <IRremoteInt.h>
#include <IRremote.h>


/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

const boolean TRACE = true;

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;
String last = "no last";

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  unsigned long value;
  static String button = "Bad Button";
  
  if (irrecv.decode(&results)) {
    value = results.value;
    // Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
    static boolean success = false;
    switch (value) {
      case 0x511dbb:
      case 0xff629d:
         button = "ON";
         success = true;
         break;
      case 0xffa25d:
      case 0xe318261b:
         button = "CH-";
         success = true;
         break;
      case 0xffe21d:
      case 0xee886d7f:
         button = "CH+";
         success = true;
         break;
      case 0x52a3d41f:
      case 0xff22dd:
         button = "|<<";
         success = true;
         break;
      case 0xd7e84b1b:
         button = ">>|";
         success = true;
         break;
      case 0x20fe4dbb:
         button = ">>||";
         success = true;
         break;
      case 0xf076c13b:
         button = "-";
         success = true;
         break;
      case 0xa3c8eddb:
         button = "+";
         success = true;
         break;
      case 0xe5cfbd7f:
         button = "EQ";
         success = true;
         break;
      case 0xffb04f:
      case 0xf0c41643:
         button = "200+";
         success = true;
         break;
      case 0xFF9867:
      case 0x97483bfb:
         button = "100+";
         success = true;
         break;
      case 0xff30cf:
      case 0x9716be3f:
         button = "1";
         success = true;
         break;
      case 0xff18e7:
      case 0x3d9ae3f7:
         button = "2";
         success = true;
         break;
      case 0xff7a85:
      case 0x6182021b:
         button = "3";
         success = true;
         break;
      case 0xff10ef:
      case 0x8c22657b:
         button = "4";
         success = true;
         break;
      case 0xff38c7:
      case 0x488f3cbb:
         button = "5";
         success = true;
         break;
      case 0xff5aa5:
      case 0x449e79f:
         button = "6";
         success = true;
         break;
      case 0xff42bd:
      case 0x32c6fdf7:
         button = "7";
         success = true;
         break;
      case 0xff4ab5:
      case 0x1bc0157b:
         button = "8";
         success = true;
         break;
      case 0xff52ad:
      case 0x3ec3fc1b:
         button = "9";
         success = true;
         break;
      case 0xff6897:
      case 0xc101e57b:
         button = "0";
         success = true;
         break;
      case 0xffffffff:
         // button = String("Repeat: " + last);
         break;
      default:
         success = false;
         button = "Unknown " + String(value, HEX);
         last   = "None";
         if (TRACE) {
          Serial.println("Debug: " + button);
         }
         break;
    }
    if (success) {
       Serial.println("You pressed: " + button);
       last = String(button);
    }
  }
  delay(100);
}
