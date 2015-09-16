/*
 * Ansible Sign Basic Example
 * Runs on Teensy 2.0 and the Arduino Yún.
 * Copyright 2015 Chris Church <chris@ninemoreminutes.com>
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

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(97, 12, NEO_GRB + NEO_KHZ800);

void setup() {
    pixels.begin();
    pixels.setBrightness(255);
    pixels.show();
}

void loop() {
    for (int i = 0; i < 97; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255));
        // ANSIBLE BLUE:  HEX #64c5c7
        //pixels.setPixelColor(i, pixels.Color(0x64, 0xc5, 0xc7));
        // ANSIBLE RED: HEX #ff5850
        //pixels.setPixelColor(i, pixels.Color(0xff, 0x58, 0x50));
    }
    pixels.show();
}

