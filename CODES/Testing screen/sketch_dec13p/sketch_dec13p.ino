//**************************************SETUP*************************************************
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <SD.h>
#include <Adafruit_GFX.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include "my_icons.h" //my icons wt arduino scree
uint16_t ID;

void setup() {
  // put your setup code here, to run once:
tft.begin(ID); //clear screen/ restart
  tft.setRotation(3);
    tft.fillScreen(0xFFFF);
  tft.drawRGBBitmap(100, 50, test, 350, 200);
      delay(15);
}

void loop() {
  // put your main code here, to run repeatedly:

}
