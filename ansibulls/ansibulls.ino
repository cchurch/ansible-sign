#include "Adafruit_WS2801.h"
#include "SPI.h"

uint8_t dataPin  = 19;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 18;    // Green wire on Adafruit Pixels

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(50, dataPin, clockPin);

// Create a 24 bit color value from RGB
uint32_t Color(byte r, byte g, byte b)
{
    uint32_t c;
    c = r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    return c;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void setup()
{
    strip.begin();
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Color(0, 0, 0));
    }
    strip.show();
}

void updateSnow()
{
    static int index = 0;

    for (int i = 0; i < strip.numPixels(); i++) {
        // LEDs 1-24 (except 22) are under the "snow" from right to left.
        if (i < 1 || i > 24 || i == 22) {
            continue;
        }
        if (index < 25 && index == i) {
            strip.setPixelColor(i, Color(255, 216, 216));
        }
        else if (index >= 25 && index < 50 && (50 - index) == i) {
            strip.setPixelColor(i, Color(255, 216, 216));
        }
        else {
            strip.setPixelColor(i, Color(164, 128, 128));
        }
    }

    if (++index >= 50) {
        index = 0;
    }
}

void updateFire()
{
    static int index = 0;

    for (int i = 0; i < strip.numPixels(); i++) {
        // LEDs 22, 25 and 26 are the fireplace.
        if (i != 22 && i != 25 && i != 26) {
            continue;
        }
        if (index < 5) {
            strip.setPixelColor(i, Color(255 - i - 3 * index, 3 * i - 6 * index, 0));
        }
        else {
            strip.setPixelColor(i, Color(255 - i - 3 * (10 - index), 3 * i - 6 * (10 - index), 0));
        }
    }

    if (++index >= 10) {
        index = 0;
    }
}

void updateTree()
{
    static int index = 0;

    for (int i = 0; i < strip.numPixels(); i++) {
        // LEDs 33-41 are the tree.
        if (i < 33 || i > 41) {
            continue;
        }
        strip.setPixelColor(i, Wheel(index));
    }

    if (++index >= 256) {
        index = 0;
    }
}

void loop()
{
    updateSnow();
    updateFire();
    updateTree();
    strip.show();
    delay(40);
}

