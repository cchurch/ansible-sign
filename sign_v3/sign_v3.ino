/*
 * Ansible Sign Blinky Lights
 * Runs on Teensy 2.0 and the Arduino Yún.
 * Copyright 2016 Chris Church <chris@ninemoreminutes.com>
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

#include "Colors.h"
#include "Patterns.h"
#include "Sign.h"

// Run in bright mode (set to 0 when working up close to avoid blinding yourself).
#define NP_BRIGHT 1

// Outside ring, clockwise.
#define NP_OUTER 52

// Inside ring, counterclockwise.
#define NP_INNER 45

Sign sign = Sign();

Color redMotion = Color(255, 0, 0, -63, 0);
Color whiteMotion = Color(255, 255, 255, -63, 0);

FadePattern i1 = FadePattern(52, 45);
FadePattern i2 = FadePattern(52, 45);

FadePattern o1 = FadePattern(0, 52);
FadePattern o2 = FadePattern(0, 52);
FadePattern o3 = FadePattern(0, 52);

Pattern* pInnerPatterns[] = {&i1, &i2, 0};
Pattern* pOuterPatterns[] = {&o1, &o2, &o3, 0};

void setup()
{
    sign.setBrightness(NP_BRIGHT ? 240 : 32);
    sign.setMotionPin(8);
    sign.setPixelCount(97);
    sign.setPixelPin(12);
    sign.setSoundInput(0);
    sign.setup();

    i1.setPrimaryColor(redMotion);
    i1.setSecondaryColor(whiteMotion);
    i2.setPrimaryColor(whiteMotion);
    i2.setSecondaryColor(redMotion);

    o1.setPrimaryColor(BLACK);
    o1.setSecondaryColor(WHITE);
    o2.setPrimaryColor(WHITE);
    o2.setSecondaryColor(RED);
    o3.setPrimaryColor(RED);
    o3.setSecondaryColor(BLACK);
}

void loop()
{
    static int innerIndex = 0;
    static int outerIndex = 0;
    static bool innerResult = false;
    static bool outerResult = false;

    Pattern *pInner = pInnerPatterns[innerIndex];
    Pattern *pOuter = pOuterPatterns[outerIndex];

    if (innerIndex >= 2) {
        innerIndex = 0;
        pInner = pInnerPatterns[0];
        innerResult = false;
    }
    if (!innerResult) {
        pInner->reset();
    }
    innerResult = pInner->tick(&sign);
    if (!innerResult) {
        innerIndex++;
    }

    if (outerIndex >= 3) {
        outerIndex = 0;
        pOuter = pOuterPatterns[0];
        outerResult = false;
    }
    if (!outerResult) {
        pOuter->reset();
    }
    outerResult = pOuter->tick(&sign);
    if (!outerResult) {
        outerIndex++;
    }
    
    sign.wait(40);
}

