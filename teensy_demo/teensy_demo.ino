/*
 * Teensy 2.0 NeoPixel Demo (based on AdaFruit simple example)
 * Copyright 2014 Chris Church <chris@ninemoreminutes.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Adafruit_NeoPixel.h"

#define NP_COUNT 60
#define NP_PIN 12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NP_COUNT, NP_PIN, NEO_GRB + NEO_KHZ800);

// 
static int i;
static int j;

void setup()
{
    pixels.begin();
}

void loop()
{
    int z = j % 8;
    int y = (2 * (j % 64)) + 64;
    int r = (z == 0 || z == 3 || z == 4 || z == 6) ? y : 0;
    int g = (z == 1 || z == 4 || z == 5 || z == 6) ? y : 0;
    int b = (z == 2 || z == 5 || z == 3 || z == 6) ? y : 0;
 
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    if (j < 128 || i == (NP_COUNT - 1)) {
        pixels.show();
    }

    if (j >= 64 && j < 128) {
        i--;
    }
    else {
        i++;
    }

    if (i >= NP_COUNT) {
        i = 0;
        j++;
    }
    else if (i < 0) {
        i = NP_COUNT - 1;
        j++;
        if (j == 128) {
            i = 0;
        }
    }
    
    if (j < 128) {
        delay((j % 64) / 8 + 4);
    }
    else if (j < 192) {
        if (i == 0) {
            delay((40 * (j % 64) / 8) + 200);
        }
    }
    else {
        j = 0;
    }
}

