/*
 * Ansible Sign Blinky Lights
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

#define NP_COUNT 97
#define NP_PIN 12

// Run in bright mode (set to 0 when working up close to avoid blinding yourself).
#define NP_BRIGHT 1

// Outside ring, clockwise.
#define NP_OUTER 52

// Inside ring, counterclockwise.
#define NP_INNER 45

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NP_COUNT, NP_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    pixels.begin();
    pixels.setBrightness(NP_BRIGHT ? 240 : 31);
}

int last_sound_level = 0;

int get_sound_level(int ms)
{
    unsigned long start_ms = millis();
    unsigned int d_min = 1024;
    unsigned int d_max = 0;
    unsigned int d_sample;

    while (millis() - start_ms < ms) {
        d_sample = analogRead(0);
        if (d_sample < 1024) {
            if (d_sample > d_max) {
                d_max = d_sample;
            }
            if (d_sample < d_min) {
                d_min = d_sample;
            }
        }
    }
    if (d_max > 511) {
        d_max = 511;
    }
    last_sound_level = (d_max - d_min) / 2;
    return last_sound_level;
}
    
// Set brightness and color based on sequence counter.
void set_color(int p, int n)
{
    int y = (2 * (n % 64)) + 64;
    int z = n % 8;
    int r = (z == 0 || z == 3 || z == 4 || z == 6) ? y : 0;
    int g = (z == 1 || z == 4 || z == 5 || z == 6) ? y : 0;
    int b = (z == 2 || z == 5 || z == 3 || z == 6) ? y : 0;
    pixels.setPixelColor(p, pixels.Color(r, g, b));
}

// Clockwise loop.
int cw_loop(int n)
{
    static int p = 0;

    set_color(p++, n);
    pixels.show();
    delay((n % 64) / 8 + 4);
    if (p >= NP_OUTER) {
        p = 0;
        n = (n < 64) ? n + 1 : -1;
    }

    return n;
}

// Counter-clockwise loop.
int ccw_loop(int n)
{
    static int p = NP_OUTER - 1;

    set_color(p--, n);
    pixels.show();
    delay((n % 64) / 8 + 4);
    if (p < 0) {
        p = NP_OUTER - 1;
        n = (n < 64) ? n + 1 : -1;
    }

    return n;
}

// Cycle through solid colors.
int solid_colors(int n)
{
    static int p = 0;

    set_color(p++, n);
    if (p >= NP_OUTER) {
        pixels.show();
        p = 0;
        delay((40 * (n % 64) / 8) + 200);
        n = (n < 64) ? n + 1 : -1;
    }

    return n;
}

int pulsing_white(int n)
{
    static int p = 0;

    int y = 38;
    if ((n % 32) < 16) {
        y += (n % 16) * 8;
    }
    else {
        y += (128 - (n % 16) * 8);
    }
    pixels.setPixelColor(p++, pixels.Color(y, y, y));
    if (p >= NP_OUTER) {
        pixels.show();
        p = 0;
        delay(100);
        n = (n < 256) ? n + 1 : -1;
    }

    return n;
}

// Ansible red/green colors.
int split_ansible_colors(int n)
{
    int y = 0;
    int p = 0;

    y = get_sound_level(50) / 4 + 128;
    //y = last_sound_level / 4 + 128;

    for (p = 0; p < NP_OUTER; p++) {
        int r, g, b;
        if (((p + n) % NP_OUTER) < (NP_OUTER / 2)) {
            r = y * 0x24 / 256;
            g = y * 0xc5 / 256;
            b = y * 0x57 / 256;
        }
        else {
            r = y;
            g = y * 0x58 / 256;
            b = y * 0x50 / 256;
        }
        pixels.setPixelColor(p, pixels.Color(r, g, b));
    }    

    pixels.show();
    n = (n < NP_OUTER * 16) ? n + 1 : -1;

    return n;
}

// Blue, orange, white.
int durham_bulls_colors(int n)
{
    static int p = 0;
    static int y = 0;

    if (p == 0) {
        y = get_sound_level(40) / 2 + 64;
    }

    int r = y;
    int g = y;
    int b = y;
    if (((p + n) % NP_OUTER) < (NP_OUTER / 3)) {
        r = 0; g = 0;
    }
    else if (((p + n) % NP_OUTER) < (2 * NP_OUTER / 3)) {
        g = 3 * y / 4; b = 0;
    }
    pixels.setPixelColor(p++, pixels.Color(r, g, b));
    if (p >= NP_OUTER) {
        pixels.show();
        p = 0;
        n = (n < NP_OUTER * 16) ? n + 1 : -1;
    }

    return n;    
}

// Pulsing white, brightness based on sound level.
int sound_activated(int n)
{
    int p = 0;
    int y = 0;

    y = get_sound_level(100) / 2 + (n / 4);
    for (p = 0; p < NP_OUTER; p++) {
        pixels.setPixelColor(p, pixels.Color(y, y, y));
    }
    pixels.show();

    return (n < 256) ? n + 1 : -1;  
}

// Pulsing white, brightness based on sound level.
int sound_activated_inner(int n)
{
    int p = 0;
    int y = 0;

    //y = get_sound_level(100) / 2 + (n / 4);
    //y = last_sound_level / 2 + (n / 4);
    //y = 255 - (get_sound_level(100) / 4 + (n / 8));
    y = 255 - (last_sound_level / 4 + (n / 8));
    for (p = NP_OUTER; p < NP_COUNT; p++) {
        pixels.setPixelColor(p, pixels.Color(y, y, y));
    }
    pixels.show();

    return (n < 256) ? n + 1 : -1;  
}

void loop()
{
    // Current sequence.
    static int s_outer = 0; 
    //static int s_inner = 0;
    
    // Sequence counter.  Starts at 0 when first passed to a sequence; the sequence
    // returns the value to be passed in the next loop.  When a sequence returns -1,
    // it has completed and we move on to the next sequence.
    static int n_outer = 0;
    static int n_inner = 0;

    switch (s_outer) {
        /*case 0:
            n_outer = cw_loop(n_outer);
            break;
        case 1:
            n_outer = ccw_loop(n_outer);
            break;
        case 2:
            n_outer = solid_colors(n_outer);
            break;
        case 3:
            n_outer = pulsing_white(n_outer);
            break;*/
        case 4:
            n_outer = split_ansible_colors(n_outer);
            break;
        /*case 5:
            n_outer = durham_bulls_colors(n_outer);
            break;*/
        /*case 6:
            n_outer = sound_activated(n_outer);
            break;*/
        case 99:
            s_outer = 0;
            break;
        default:
            n_outer = -1;
            break;
    }
    if (n_outer < 0) {
        s_outer++;
        n_outer = 0;
    }

    n_inner = sound_activated_inner(n_inner);
    if (n_inner < 0) {
        n_inner = 0;
    }

}
