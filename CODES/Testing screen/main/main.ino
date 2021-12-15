//**************************************SETUP*************************************************
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <SD.h>
#include <Adafruit_GFX.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <DHT.h>
//Constants
#define DHTPIN A7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//call tft.setFont() to return to default
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSansBold9pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"
#include "FreeDefaultFonts.h"

//#include "Ard_Logo.h" //ardunio screen
#include "my_icons.h" //my icons wt arduino screen

//it is 16bits color coded values. Different from the normal 0-255 24bits
//PACKED BCD, 15-11 = Red bit | 10-5 = Green bit | 4 - 0 bit = Blue bit
#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410

//SD stage
// would I be able to do switching background?
#define SD_CS 10
#define NAMEMATCH ""        // use ""  to matche with any name in SD Root dir
#define PALETTEDEPTH   0    // do not support Palette modes
char namebuf[32] = "/";     //BMP files in root directory
File root;
int pathlen;
uint8_t         spi_save;
bool good;    //if SD has been read
uint16_t ID;
//**************************************END**OF**SETUP****************************************

//****************************************MAIN************************************************
void setup() {

  pinMode(A7, INPUT);// for Temperature


  Serial.begin(9600);
  //  while (!Serial) {
  //  }
  Serial.print("Show BMP files on TFT with ID:0x");
  ID = tft.readID();
  Serial.println(ID, HEX);

  //Start up screen:
  run_arduino();

  tft.begin(ID);
  tft.setFont();
  tft.setRotation(3);// 270 degress
  tft.setTextColor(0xFFFF, 0x0000);

  good = SD.begin(SD_CS);
  if (!good) {
    Serial.println(F("cannot start SD"));
    tft.fillScreen(0x000F); //fillScreen(uint16_t t); 0x000F = Navy
    tft.setCursor(45, 160);
    tft.setTextColor(0xFFFF, 0x0000); //setTextColor(uint16_t text, uint16_t background)
    tft.setTextSize(2); //setTextSize(uint8_t s) s = 1 to 5
    tft.println("ERROR: No SD Card Found...");
    delay(1000); //wait a sec

    //Reload and start loading default
    tft.fillScreen(0x000F); //fillScreen(uint16_t t); 0x000F = Navy
    tft.setCursor(45, 160);
    tft.println("Starting with default background..");
    delay(1000);
  }
  else {
    //READ SD card
    root = SD.open(namebuf);
    pathlen = strlen(namebuf);
  }

  //always run loading
  dht.begin();
  run_loading();

  //return to normal
  tft.begin(ID);
  tft.setRotation(3);// 270 degress
  tft.setCursor(45, 160);
  tft.setTextSize(2);
  tft.setFont();

}

int chk;
float hum;  //Stores humidity value
float tempC; //Stores temperature value
float tempF; //Stores temperature value

void loop() {
  Serial.println("PRINTED WHITE");
  tft.fillScreen(WHITE); // clear screen
  //begin again, to remove invert

  if (good) { //use sd card background
    Serial.println("starting PHOTOTS");
    display_SD();
  }
  else //use default background
  {
    Serial.println("PRINTED GREEN");
    //*****THIS WORKS TO PRINT OUR HEX ARRAY OF AN IMAGE***********
//      tft.setCursor(160, 330); //This doesnt matter
      int h = 150, w = 159, row, col, buffidx = 0;
      int xpos = 160, ypos = 330; //x with width, y with hieght from w=480, h=320
      for (row = 0 + xpos; row < h + xpos; row++) { // For each scanline...
        for (col = 0 + ypos; col < w + ypos; col++) { // For each pixel...
          //To read from Flash Memory, pgm_read_XXX is required.
          //Since image is stored as uint16_t, pgm_read_word is used as it uses 16bit address
          tft.drawPixel(col, row, pgm_read_word(og + buffidx));
          buffidx++;
        } // end pixel
      }
    //************************************************************

  }

  Serial.println("PRINTED THEN I PRINT");
  hum = dht.readHumidity();
  tempC = dht.readTemperature();
  tempF = (tempC * 1.8) + 32;
  int x = 45, y = 100;
  //Print temp and humidity values to serial monitor
  tft.setCursor(x, y);
  tft.print("Humidity: ");  tft.print(hum); tft.println("%");// xx (degreesymbol) % Humidity
  tft.setCursor(x + 120, y + 20);
  tft.print(tempC); tft.print((char)247); tft.println("C"); // xx (degreesymbol) C
  tft.setCursor(x + 120, y + 20 + 20 );
  tft.print(tempF);  tft.print((char)247); tft.println("F"); // xx (degreesymbol) F
  delay(2000); //Delay 2 sec.
}
//****************************************MAIN************************************************
//**************************************DISPLAY_SD********************************************
void display_SD() {
  char *nm = namebuf + pathlen;
  File f = root.openNextFile();
  uint8_t ret;
  uint32_t start;
  if (f == NULL) {
    root.rewindDirectory();
    f = root.openNextFile();
  }

  if (f != NULL) {
#ifdef USE_SDFAT
    f.getName(nm, 32 - pathlen);
#else
    strcpy(nm, (char *)f.name());
    if (strstr(nm, "SYSTEM~1") != NULL) {
      f = root.openNextFile();
      strcpy(nm, (char *)f.name());
    }
#endif
    f.close();

    strlwr(nm);
    Serial.println(strstr(nm, ".bmp"));
    Serial.println(strstr(nm, NAMEMATCH));
    if (strstr(nm, ".bmp") != NULL && strstr(nm, NAMEMATCH) != NULL) {
      Serial.print(namebuf);
      Serial.print(F(" - "));
      tft.fillScreen(0);
      start = millis();
      ret = showBMP(namebuf, 0, 0);
      //        showBMP(namebuf,5,5);
      switch (ret) {
        case 0:
          Serial.print(millis() - start);
          Serial.println(F("ms"));
          delay(0);
          break;
        case 1:
          Serial.println(F("bad position"));
          break;
        case 2:
          Serial.println(F("bad BMP ID"));
          break;
        case 3:
          Serial.println(F("wrong number of planes"));
          break;
        case 4:
          Serial.println(F("unsupported BMP format"));
          break;
        case 5:
          Serial.println(F("unsupported palette"));
          break;
        default:
          Serial.println(F("unknown"));
          break;
      }
    }

  }
  else root.rewindDirectory();

  Serial.print("============= WHAT DID I PRINT?: ");
  Serial.print(nm);
  Serial.println(namebuf);
}
#define BMPIMAGEOFFSET 54

#define BUFFPIXEL      20

uint16_t read16(File& f) {
  uint16_t result;         // read little-endian
  f.read((uint8_t*)&result, sizeof(result));
  return result;
}

uint32_t read32(File& f) {
  uint32_t result;
  f.read((uint8_t*)&result, sizeof(result));
  return result;
}

uint8_t showBMP(char *nm, int x, int y)
{
  File bmpFile;
  int bmpWidth, bmpHeight;    // W+H in pixels
  uint8_t bmpDepth;           // Bit depth (currently must be 24, 16, 8, 4, 1)
  uint32_t bmpImageoffset;    // Start of image data in file
  uint32_t rowSize;           // Not always = bmpWidth; may have padding
  uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel in buffer (R+G+B per pixel)
  uint16_t lcdbuffer[(1 << PALETTEDEPTH) + BUFFPIXEL], *palette = NULL;
  uint8_t bitmask, bitshift;
  boolean flip = true;        // BMP is stored bottom-to-top
  int w, h, row, col, lcdbufsiz = (1 << PALETTEDEPTH) + BUFFPIXEL, buffidx;
  uint32_t pos;               // seek position
  boolean is565 = false;      //

  uint16_t bmpID;
  uint16_t n;                 // blocks read
  uint8_t ret;

  if ((x >= tft.width()) || (y >= tft.height()))
    return 1;               // off screen

  bmpFile = SD.open(nm);      // Parse BMP header
  bmpID = read16(bmpFile);    // BMP signature
  (void) read32(bmpFile);     // Read & ignore file size
  (void) read32(bmpFile);     // Read & ignore creator bytes
  bmpImageoffset = read32(bmpFile);       // Start of image data
  (void) read32(bmpFile);     // Read & ignore DIB header size
  bmpWidth = read32(bmpFile);
  bmpHeight = read32(bmpFile);
  n = read16(bmpFile);        // # planes -- must be '1'
  bmpDepth = read16(bmpFile); // bits per pixel
  pos = read32(bmpFile);      // format
  if (bmpID != 0x4D42) ret = 2; // bad ID
  else if (n != 1) ret = 3;   // too many planes
  else if (pos != 0 && pos != 3) ret = 4; // format: 0 = uncompressed, 3 = 565
  else if (bmpDepth < 16 && bmpDepth > PALETTEDEPTH) ret = 5; // palette
  else {
    bool first = true;
    is565 = (pos == 3);               // ?already in 16-bit format
    // BMP rows are padded (if needed) to 4-byte boundary
    rowSize = (bmpWidth * bmpDepth / 8 + 3) & ~3;
    if (bmpHeight < 0) {              // If negative, image is in top-down order.
      bmpHeight = -bmpHeight;
      flip = false;
    }

    w = bmpWidth;
    h = bmpHeight;
    if ((x + w) >= tft.width())       // Crop area to be loaded
      w = tft.width() - x;
    if ((y + h) >= tft.height())      //
      h = tft.height() - y;

    if (bmpDepth <= PALETTEDEPTH) {   // these modes have separate palette
      //bmpFile.seek(BMPIMAGEOFFSET); //palette is always @ 54
      bmpFile.seek(bmpImageoffset - (4 << bmpDepth)); //54 for regular, diff for colorsimportant
      bitmask = 0xFF;
      if (bmpDepth < 8)
        bitmask >>= bmpDepth;
      bitshift = 8 - bmpDepth;
      n = 1 << bmpDepth;
      lcdbufsiz -= n;
      palette = lcdbuffer + lcdbufsiz;
      for (col = 0; col < n; col++) {
        pos = read32(bmpFile);    //map palette to 5-6-5
        palette[col] = ((pos & 0x0000F8) >> 3) | ((pos & 0x00FC00) >> 5) | ((pos & 0xF80000) >> 8);
      }
    }

    // Set TFT address window to clipped image bounds
    tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
    for (row = 0; row < h; row++) { // For each scanline...
      // Seek to start of scan line.  It might seem labor-
      // intensive to be doing this on every line, but this
      // method covers a lot of gritty details like cropping
      // and scanline padding.  Also, the seek only takes
      // place if the file position actually needs to change
      // (avoids a lot of cluster math in SD library).
      uint8_t r, g, b, *sdptr;
      int lcdidx, lcdleft;
      if (flip)   // Bitmap is stored bottom-to-top order (normal BMP)
        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
      else        // Bitmap is stored top-to-bottom
        pos = bmpImageoffset + row * rowSize;
      if (bmpFile.position() != pos) { // Need seek?
        bmpFile.seek(pos);
        buffidx = sizeof(sdbuffer); // Force buffer reload
      }

      for (col = 0; col < w; ) {  //pixels in row
        lcdleft = w - col;
        if (lcdleft > lcdbufsiz) lcdleft = lcdbufsiz;
        for (lcdidx = 0; lcdidx < lcdleft; lcdidx++) { // buffer at a time
          uint16_t color;
          // Time to read more pixel data?
          if (buffidx >= sizeof(sdbuffer)) { // Indeed
            bmpFile.read(sdbuffer, sizeof(sdbuffer));
            buffidx = 0; // Set index to beginning
            r = 0;
          }
          switch (bmpDepth) {          // Convert pixel from BMP to TFT format
            case 24:
              b = sdbuffer[buffidx++];
              g = sdbuffer[buffidx++];
              r = sdbuffer[buffidx++];
              color = tft.color565(r, g, b);
              break;
            case 16:
              b = sdbuffer[buffidx++];
              r = sdbuffer[buffidx++];
              if (is565)
                color = (r << 8) | (b);
              else
                color = (r << 9) | ((b & 0xE0) << 1) | (b & 0x1F);
              break;
            case 1:
            case 4:
            case 8:
              if (r == 0)
                b = sdbuffer[buffidx++], r = 8;
              color = palette[(b >> bitshift) & bitmask];
              r -= bmpDepth;
              b <<= bmpDepth;
              break;
          }
          lcdbuffer[lcdidx] = color;

        }
        tft.pushColors(lcdbuffer, lcdidx, first);
        first = false;
        col += lcdidx;
      }           // end cols
    }               // end rows
    tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1); //restore full screen
    ret = 0;        // good render
  }
  bmpFile.close();
  return (ret);
}
//**************************************DISPLAY_SD********************************************
//**************************************Arduino**Screen***************************************
uint8_t r = 255, g = 255, b = 255;
uint16_t color;
void arduino_showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
  int16_t x1, y1;
  uint16_t wid, ht;
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextSize(sz);
  tft.println(msg);
}
void run_arduino()
{
  tft.begin(ID); //clear screen/ restart
  tft.setRotation(3);
  //  tft.invertDisplay(true);
  tft.fillScreen(WHITE);
  //*****THIS WORKS TO PRINT OUR HEX ARRAY OF AN IMAGE***********
  //  tft.setCursor(156, 100);
  //  int h = 144, w = 256, row, col, buffidx = 0;
  //  int xpos = 110, ypos = 85;
  //  for (row = 0 + ypos; row < h + ypos; row++) { // For each scanline...
  //    for (col = 0 + xpos; col < w + xpos; col++) { // For each pixel...
  //      //To read from Flash Memory, pgm_read_XXX is required.
  //      //Since image is stored as uint16_t, pgm_read_word is used as it uses 16bit address
  //      tft.drawPixel(col, row, pgm_read_word(test + buffidx));
  //      buffidx++;
  //    } // end pixel
  //  }
  //************************************************************
  // tft.drawRGBBitmap(100, 50, testing1, 350, 200);
  tft.drawRGBBitmap(100, 50, Logo, 350, 200);
  delay(1000);
  tft.setTextSize(2);
  for (int j = 0; j < 20; j++) {
    color = tft.color565(r -= 12, g -= 12, b -= 12);
    tft.setTextColor(color);
    arduino_showmsgXY(135, 260, 1, &FreeSansBold9pt7b, "A TYLON'S INVENTION");
    delay(15);
  }
  delay(900);
  //  tft.drawRGBBitmap(100, 50, test, 350, 200);
  delay(100);
  for (int i = 0; i < 480; i++) {
    tft.vertScroll(0, 480, i);
    tft.drawFastVLine(i, 0, 320, 0xffff); // vertical lines to push out screen
    delay(1);
  }
}
//**************************************Arduino**Screen***************************************

//**************************************Loading**Screen***************************************
#define PI 3.1415926535897932384626433832795
int col[8];


void loading_showmsgXY(int x, int y, int sz, const GFXfont * f, const char *msg)
{
  int16_t x1, y1;
  uint16_t wid, ht;
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(0x0000);
  tft.setTextSize(sz);
  tft.print(msg);
}

void run_loading() {
  tft.begin(ID); //auto reset & clrs screen
  tft.setRotation(3);
  tft.invertDisplay(true); //for the nice back color
  tft.fillScreen(0xffff);
  loading_showmsgXY(170, 250, 2, &FreeSans9pt7b, "Loading...");
  col[0] = tft.color565(155, 0, 50);
  col[1] = tft.color565(170, 30, 80);
  col[2] = tft.color565(195, 60, 110);
  col[3] = tft.color565(215, 90, 140);
  col[4] = tft.color565(230, 120, 170);
  col[5] = tft.color565(250, 150, 200);
  col[6] = tft.color565(255, 180, 220);
  col[7] = tft.color565(255, 210, 240);

  //loop portion
  for (int i = 8 * 3; i > 0; i--) { //repeat 3 times
    tft.fillCircle(240 + 40 * (cos(-i * PI / 4)), 120 + 40 * (sin(-i * PI / 4)), 10, col[0]); delay(15);
    tft.fillCircle(240 + 40 * (cos(-(i + 1)*PI / 4)), 120 + 40 * (sin(-(i + 1)*PI / 4)), 10, col[1]); delay(0);
    tft.fillCircle(240 + 40 * (cos(-(i + 2)*PI / 4)), 120 + 40 * (sin(-(i + 2)*PI / 4)), 10, col[2]); delay(0);
    tft.fillCircle(240 + 40 * (cos(-(i + 3)*PI / 4)), 120 + 40 * (sin(-(i + 3)*PI / 4)), 10, col[3]); delay(0);
    tft.fillCircle(240 + 40 * (cos(-(i + 4)*PI / 4)), 120 + 40 * (sin(-(i + 4)*PI / 4)), 10, col[4]); delay(0);
    tft.fillCircle(240 + 40 * (cos(-(i + 5)*PI / 4)), 120 + 40 * (sin(-(i + 5)*PI / 4)), 10, col[5]); delay(0);
    tft.fillCircle(240 + 40 * (cos(-(i + 6)*PI / 4)), 120 + 40 * (sin(-(i + 6)*PI / 4)), 10, col[6]); delay(0);
    tft.fillCircle(240 + 40 * (cos(-(i + 7)*PI / 4)), 120 + 40 * (sin(-(i + 7)*PI / 4)), 10, col[7]); delay(0);
  }
}


//**************************************Loading**Screen***************************************
