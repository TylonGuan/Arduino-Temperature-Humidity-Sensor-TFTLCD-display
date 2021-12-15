//Demo Function: Display bmp file from TF card
//Arduino IDE: 1.6.5
// Board: Arduino UNO R3, Arduino Mega2560

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <SD.h>
#include <Adafruit_GFX.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;
#define SD_CS 10
#define NAMEMATCH ""        // "" matches any name

File root;
char namebuf[32] = "/";
int16_t wid; int16_t ht;
int pathlen;
uint8_t         spi_save;
bool good;
void setup()
{
  uint16_t ID;
  Serial.begin(9600);
  while (!Serial) {

  }
  Serial.print("Show BMP files on TFT with ID:0x");
  ID = tft.readID();
  Serial.println(ID, HEX);
  if (ID == 0x0D3D3) ID = 0x9481;
  tft.begin(ID);
  wid = tft.width();
  ht = tft.height();
  Serial.print("Size of Display: "); Serial.print(wid); Serial.print("*"); Serial.println(ht);
  tft.fillScreen(0x0000);
  tft.setTextColor(0xFFFF, 0x0000);
  tft.setRotation(3);
  /*  if (tft.height() > tft.width()) tft.setRotation(1);    //LANDSCAPE
    tft.setTextColor(0xFFFF, 0x0000);*/
  good = SD.begin(SD_CS);
  if (!good) {
    Serial.println(F("cannot start SD"));
    tft.fillScreen(0x000F); //fillScreen(uint16_t t); 0x000F = Navy
    tft.setCursor(45, 160);
    tft.setTextColor(0xFFFF, 0x0000); //setTextColor(uint16_t text, uint16_t background)
    tft.setTextSize(2); //setTextSize(uint8_t s) s = 1 to 5
    tft.println("ERROR: No SD Card Found...");
    //    while (1);
  }
  else {
    root = SD.open(namebuf);
    pathlen = strlen(namebuf);
  }

}

void loop()
{
  if (good) {
    char *nm = namebuf + pathlen;
    File f = root.openNextFile();
    uint8_t ret;
    uint32_t start;
    if (f != NULL) {
#ifdef USE_SDFAT
      f.getName(nm, 32 - pathlen);
#else
      strcpy(nm, (char *)f.name());
#endif
      f.close();
      strlwr(nm);
      if (strstr(nm, ".bmp") != NULL && strstr(nm, NAMEMATCH) != NULL) {
        Serial.print(namebuf);
        Serial.print(F(" - "));
        tft.fillScreen(0x0000);
        start = millis();
        bmpDraw(namebuf, 0, 0);
      }
    }
    else root.rewindDirectory();
  } //end of Reading SD
  else {
    //    extern const uint8_t penguin[];
    //    bmpDraw(penguin, 0,0);
//    tft.loadImage("Cyan_BMP.bmp");
  }//end of loading from memory

}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void bmpDraw(char *filename, int x, int y) {
  File   bmpFile;
  int    bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;        // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;      // Start of image data in file
  uint32_t rowSize;         // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel in buffer (R+G+B per pixel)
  uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;     // Set to true on valid header parse
  boolean  flip  = true;      // BMP is stored bottom-to-top
  int    w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  uint8_t  lcdidx = 0;
  boolean  first = true;

  if ((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');
  // Open requested file on SD card
  //  SPCR = spi_save;
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip    = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())  w = tft.width()  - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        //        SPCR = 0;
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

        for (row = 0; row < h; row++) { // For each scanline...
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else   // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          //          SPCR = spi_save;
          if (bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col = 0; col < w; col++) { // For each column...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              // Push LCD buffer to the display first
              if (lcdidx > 0) {
                //                SPCR = 0;
                tft.pushColors(lcdbuffer, lcdidx, first);
                lcdidx = 0;
                first  = false;
              }
              //              SPCR = spi_save;
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx++] = tft.color565(r, g, b);
          } // end pixel
        } // end scanline
        // Write any remaining data to LCD
        if (lcdidx > 0) {
          //          SPCR = 0;
          tft.pushColors(lcdbuffer, lcdidx, first);
        }
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }
  else {
    tft.fillScreen(0x000F); //fillScreen(uint16_t t); 0x000F = Navy
    tft.setCursor(45, 160);
    tft.setTextColor(0xFFFF, 0x0000); //setTextColor(uint16_t text, uint16_t background)
    tft.setTextSize(2); //setTextSize(uint8_t s) s = 1 to 5
    tft.println("ERROR: BMP format not recognized");

    if (!goodBmp) Serial.println("ERROR: BMP format not recognized.");
  }
  bmpFile.close();
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
