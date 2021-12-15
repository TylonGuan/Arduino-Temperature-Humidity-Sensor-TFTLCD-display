#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"

//#include "bitmap_mono.h" // when you want to display a bitmap image from library
//#include "bitmap_RGB.h" // when you want to display a bitmap image from library
#include "Fonts/FreeSans9pt7b.h"    // when you want other fonts
#include "Fonts/FreeSans12pt7b.h" // when you want other fonts
#include "Fonts/FreeSerif12pt7b.h" // when you want other fonts
#include "FreeDefaultFonts.h" // when you want other fonts 
#include "SPI.h"  // using sdcard for display bitmap image
#include "SD.h"

//(int CS=A3, int RS=A2, int WR=A1, int RD=A0, int RST=A4)
MCUFRIEND_kbv tft(A0, A1, A2, A3, A6);


void setup() {
  Serial.begin(9600); //allow to print to serial/com window

  uint16_t ID = tft.readID();
  
 
  Serial.print("ID: "); Serial.println(ID, HEX);
  ID = 0x9486;
  Serial.print("FORCE ID: ");Serial.println(ID, HEX);
  tft.begin(ID);

  
  //int16_t width(void);
  tft.height(); //int16_t height(void);
  Serial.print("TFT Width*Height: "); Serial.print(tft.width()); Serial.print("*"); Serial.print(tft.height());

  tft.setRotation(1); //setRotation(uint8_t r)0=0, 1=90, 2=180, 3=270.
  tft.fillScreen(0x000F); //fillScreen(uint16_t t); 0x000F = Navy
  tft.setCursor(45, 160);
//  tft.setTextColor(0x0000); //setTextColor(uint16_t text)
  tft.setTextColor(0xFFFF, 0x0000); //setTextColor(uint16_t text, uint16_t background)
  tft.setTextSize(2); //setTextSize(uint8_t s) s = 1 to 5
  tft.println("ERROR: file could not be loaded");
//  tft.write(c); //write(uint8_t c) Displays a single character
  
}

void loop() {
  // put your main code here, to run repeatedly:
  tft.fillScreen(0x000F); //fillScreen(uint16_t t); 0x000F = Navy
  tft.setCursor(45, 160);
  tft.println("ERROR: file could not be loaded");
  delay(30);

}
