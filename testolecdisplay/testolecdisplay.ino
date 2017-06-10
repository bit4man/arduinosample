
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(200);
  display.clearDisplay();
}

void testDisplay(int address, byte data) {
  char buf[20];
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.print("Address:");
  display.setCursor(0,18);
  display.setTextColor(INVERSE);
  sprintf(buf,"0x%04X", address);
  display.print(buf);

  display.setTextColor(WHITE);
  display.setCursor(0,34);
  
  sprintf(buf,"Data:\n0x%02X",data);
  display.println(buf);
  display.display();
}

byte testNum;
int addr;

void loop() {
   testDisplay(addr++, testNum++);
   delay(100);
}
