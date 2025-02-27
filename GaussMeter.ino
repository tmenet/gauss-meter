/**************************************************************************
  This is a gaussmeter that automatically stores max values and display


  This code is for a 128x64 pixel display using SPI to communicate
  4 or 5 pins are required to interface.

 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SUPPLYVOLTAGE 4.5  // sensor supply voltage 

// SELKECT HALL EFFECT
// Comment out all unused parts with //

#define AH49H
//#define SS495A
//#define testOutput


// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


int volt;// read the input
double voltage;
int Gauss;
int loopCounter;
int maxGauss;
int offset;
unsigned long last_time;


void setup() {
  Serial.begin(115200);

  volt = AverageHallSensor() ;// read the input
  offset = (SUPPLYVOLTAGE * 500) - (map(volt, 0, 1023, 0, (SUPPLYVOLTAGE * 1000))); // set the correction offset value
  delay(500);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  last_time = millis();



  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();

  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...
  //testdrawrect();      // Draw rectangles (outlines)
  //testdrawchar();      // Draw characters of the default font
  //testscrolltext();    // Draw scrolling text
  //testdrawbitmap();    // Draw a small bitmap image
  //testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
}




void loop() {
  volt = AverageHallSensor() ;// read the input
  voltage = map(volt, 0, 1023, 0, (SUPPLYVOLTAGE * 1000)) + offset; // map 0-1023 to 0-SUPPLYVOLTAGE*1000 (5000 or 3300) and add correction offset
  voltage /= 1000; // divide by 1000 to get the decimal values

#ifdef AH49H
  Gauss = -(voltage - ((double)SUPPLYVOLTAGE / 2)) * 3030; // 0.33mV/Gauss is 3030.3 Gauss per Volt
#endif

#ifdef SS495A
  Gauss = (voltage - ((double)SUPPLYVOLTAGE / 2)) * 320; // 3.125mV/Gauss is 320 Gauss per Volt
#endif

  if (abs(Gauss) > abs(maxGauss))
  {
    maxGauss = Gauss;
    loopCounter = 20;
  }


// Lets update Display ever 500ms
  if ((500 + last_time) <= millis())
  {
    last_time = millis();

    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(BLACK, WHITE);        // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner
    display.println(F("GaussMeter"));
    display.setTextColor(WHITE);        // Draw white text
    display.println();
    display.setCursor(24, 32);
    display.println(Gauss);

    if (loopCounter-- <= 0) maxGauss = Gauss;
    display.setCursor(24, 46);
    display.println(maxGauss);
    display.setCursor(80, 32);
    //display.println(F(" G"));
    if (voltage < (double)SUPPLYVOLTAGE / 2)
    {
      display.println("N");
    }
    else  display.println("S");
    display.setCursor(80, 46);
    display.println(F("Max"));

    //display.println(offset);
    display.display();


#ifdef testOutput
    Serial.println(offset);
    Serial.println(voltage,4);
    Serial.println(Gauss);
#endif
  }
}


int AverageHallSensor(void)

{
  long  avgVolts=0;

  for ( byte i = 0; i < 32; i++)
  {
    avgVolts = analogRead(A1) + avgVolts;
  }

  avgVolts = avgVolts / 32;

  return avgVolts;
}
















void testdrawline() {
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  for (i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for (i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width() - 1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for (i = 0; i < display.width(); i += 4) {
    display.drawLine(0, display.height() - 1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for (i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for (i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for (i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for (i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, WHITE);
    display.display();
    delay(1);
  }
  for (i = 0; i < display.width(); i += 4) {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, WHITE);
    display.display();
    delay(1);
  }

  delay(2000); // Pause for 2 seconds
}

void testdrawrect(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testfillrect(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testdrawcircle(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < max(display.width(), display.height()) / 2; i += 2) {
    display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillcircle(void) {
  display.clearDisplay();

  for (int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}

void testdrawroundrect(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i,
                          display.height() / 4, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillroundrect(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i,
                          display.height() / 4, INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawtriangle(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < max(display.width(), display.height()) / 2; i += 5) {
    display.drawTriangle(
      display.width() / 2  , display.height() / 2 - i,
      display.width() / 2 - i, display.height() / 2 + i,
      display.width() / 2 + i, display.height() / 2 + i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfilltriangle(void) {
  display.clearDisplay();

  for (int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width() / 2  , display.height() / 2 - i,
      display.width() / 2 - i, display.height() / 2 + i,
      display.width() / 2 + i, display.height() / 2 + i, INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for (int16_t i = 0; i < 256; i++) {
    if (i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  display.setTextSize(2);
  display.println(F("Gauss"));

  display.setTextColor(WHITE); // Draw 'inverse' text
  display.println(Gauss);

  // Draw 2X-scale text
  //display.setTextColor(WHITE);
  if (voltage < 2.5)
  {
    display.println("NORTH");
  }
  else  display.println("South");
  //display.print(F("0x")); display.println(0xDEADBEEF, HEX);
  display.println(voltage);
  display.display();
  delay(2000);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}


