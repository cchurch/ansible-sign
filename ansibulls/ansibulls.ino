#include "Adafruit_WS2801.h"
#include "SPI.h"

uint8_t dataPin  = 19;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 18;    // Green wire on Adafruit Pixels

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(50, dataPin, clockPin);


// Create a 24 bit color value from RGB
static uint32_t Color(byte r, byte g, byte b)
{
    uint32_t c;
    c = r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    return c;
}


void setup()
{
    strip.begin();
    strip.show();
}


void loop()
{
    int i;

    for (i = 0; i < strip.numPixels(); i++) {
        // LEDs 1-24 (except 22) are under the "snow" from right to left.
        if (i >= 1 && i <= 24 && i != 22) {
            strip.setPixelColor(i, Color(164, 128, 128));
        }
        // LEDs 22, 25 and 26 are the fireplace.
        else if (i == 22 || i == 25 || i == 26) {
            strip.setPixelColor(i, Color(255 - i, 3 * i, 0));
        }
        // LEDs 33-41 are the tree.
        else if (i >= 33 && i <= 41) {
            strip.setPixelColor(i, Color(0, 255, 0));
        }
        // LED 48 is the red nose.
        else if (i == 48) {
            strip.setPixelColor(i, Color(255, 0, 0));
        }
        // All others are off/black.
        else {
            strip.setPixelColor(i, Color(0, 0, 0));
        }

    }
    strip.show();

    delay(40);
}

