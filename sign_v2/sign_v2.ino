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

// Motion sensor pin.
#define PIR_PIN 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NP_COUNT, NP_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    pixels.begin();
    pixels.setBrightness(NP_BRIGHT ? 240 : 31);
    pinMode(PIR_PIN, INPUT);
    digitalWrite(PIR_PIN, LOW); 
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

int get_motion()
{
    return digitalRead(PIR_PIN);
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

#define RGB_BLACK { r = g = b = 0; }
#define RGB_WHITE { r = g = b = y; }
#define RGB_RED { r = y; g = b = 0; }
#define RGB_BLUE { r = g = 0; b = y; }
#define RGB_ANSIBLE_RED { r = y; g = y * 0x58 / 256; b = y * 0x50 / 256; }
#define RGB_ANSIBLE_GREEN { r = y * 0x24 / 256; g = y * 0xc5 / 256; b = y * 0x57 / 256; }

// Ansible red/green/white colors.
int spinning_ansible_colors(int n, int s)
{
    int y = 0;
    int p = 0;

    y = get_sound_level(10) / 4 + 64;
    if (get_motion()) {
        y += 64;
    }
 
    for (p = 0; p < NP_OUTER; p++) {
        int r, g, b, q;
        if (p == (n % NP_OUTER) || p < (n / NP_OUTER)) {
            q = s;
        }
        else {
            q = s + 3;
        }
        switch (q % 4) {
            case 0:
                RGB_ANSIBLE_GREEN;
                break;
            case 1:
                RGB_ANSIBLE_RED;
                break;
            case 2:
                RGB_WHITE;
                break;
            default:
                RGB_BLACK;
                break;
        }
        if (s <= 3) {
            pixels.setPixelColor((p + 3) % NP_OUTER, pixels.Color(r, g, b));
        }
        else {
            pixels.setPixelColor((NP_OUTER - p + 2) % NP_OUTER, pixels.Color(r, g, b));
        }
    }

    pixels.show();
    if ((n % NP_OUTER) < (n / NP_OUTER)) {
        n += (n / NP_OUTER) - (n % NP_OUTER);
    }
    n = (n < (NP_OUTER * NP_OUTER)) ? n + 1 : -1;

    return n;
}

// Red/white/blue colors.
int spinning_rwb_colors(int n, int s)
{
    int y = 0;
    int p = 0;

    y = get_sound_level(10) / 4 + 128;
    //y = 255;

    for (p = 0; p < NP_OUTER; p++) {
        int r, g, b, q;
        if (p == (n % NP_OUTER) || p < (n / NP_OUTER)) {
            q = s;
        }
        else {
            q = s + 5;
        }
        switch (q % 6) {
            case 0:
                RGB_RED;
                break;
            case 2:
                RGB_WHITE;
                break;
            case 4:
                RGB_BLUE;
                break;
            default:
                RGB_BLACK;
                break;
        }
        if (s <= 5) {
            pixels.setPixelColor((p + 3) % NP_OUTER, pixels.Color(r, g, b));
        }
        else {
            pixels.setPixelColor((NP_OUTER - p + 2) % NP_OUTER, pixels.Color(r, g, b));
        }
    }

    pixels.show();
    if ((n % NP_OUTER) < (n / NP_OUTER)) {
        n += (n / NP_OUTER) - (n % NP_OUTER);
    }
    n = (n < (NP_OUTER * NP_OUTER)) ? n + 1 : -1;

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
    if (!get_motion()) {
        y -= 128;
    }

    for (p = NP_OUTER; p < NP_COUNT; p++) {
        pixels.setPixelColor(p, pixels.Color(y, y, y));
    }
    pixels.show();

    return (n < 256) ? n + 1 : -1;  
}

// Flickering Flames.
int flickering_flames(int n)
{
    int y = 0;
    int p = 0;

    y = get_sound_level(50);
    //y = last_sound_level / 4 + 128;

    for (p = 0; p < NP_OUTER;) {
        int r, g, b;
        r = (7 * p * y) % 128 + 128;
        g = (5 * p * y) % 96 + 160;
        g = (g > r) ? r : g;
        b = 0;
        pixels.setPixelColor(p, pixels.Color(r, g, b));
        p += (n % 9) + 1;
    }    

    pixels.show();
    n = (n < NP_OUTER * 16) ? n + 1 : -1;

    return n;
}

// Flickering Flames.
int flickering_flames_inner(int n)
{
    int y = 0;
    int p = 0;

    y = last_sound_level;

    for (p = NP_OUTER; p < NP_COUNT;) {
        int r, g, b;
        r = (7 * p * y) % 128 + 128;
        g = (5 * p * y) % 96 + 160;
        g = (g > r) ? r : g;
        b = 0;
        pixels.setPixelColor(p, pixels.Color(r, g, b));
        p += (n % 3) + 1;
    }    

    pixels.show();
    n = (n < NP_OUTER * 16) ? n + 1 : -1;

    return n;
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
        /*case 4:
            n_outer = split_ansible_colors(n_outer);
            break;*/
        /*case 5:
            n_outer = durham_bulls_colors(n_outer);
            break;*/
        /*case 6:
            n_outer = sound_activated(n_outer);
            break;*/
        /*case 7:
          n_outer = flickering_flames(n_outer);
          break;*/
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            n_outer = spinning_ansible_colors(n_outer, s_outer - 10);
            break;
        /*case 18:
        case 19:
        case 20:
        case 21:
        case 22:
            n_outer = spinning_rwb_colors(n_outer, s_outer - 10);
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
    //n_inner = flickering_flames_inner(n_inner);
    if (n_inner < 0) {
        n_inner = 0;
    }

}
