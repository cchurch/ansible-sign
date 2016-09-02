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

Color redMotion = Color(255, 0, 0, 64, 64);
Color whiteMotion = Color(255, 255, 255, 64, 64);

FadePattern i1 = FadePattern(52, 45, redMotion, whiteMotion);
FadePattern i2 = FadePattern(52, 45, whiteMotion, redMotion);

SolidPattern d1 = SolidPattern(0, 52);
SolidPattern d2 = SolidPattern(0, 52);
SolidPattern d3 = SolidPattern(0, 52);

FadePattern f1 = FadePattern(0, 52);
FadePattern f2 = FadePattern(0, 52);
FadePattern f3 = FadePattern(0, 52);
FadePattern f4 = FadePattern(0, 52);
FadePattern f5 = FadePattern(0, 52);
FadePattern f6 = FadePattern(0, 52);

SpinPattern s1 = SpinPattern(0, 52);
SpinPattern s2 = SpinPattern(0, 52);
SpinPattern s3 = SpinPattern(0, 52);
SpinPattern s4 = SpinPattern(0, 52);
SpinPattern s5 = SpinPattern(0, 52);
SpinPattern s6 = SpinPattern(0, 52);

//HalvesPattern h1 = HalvesPattern(0, 52);

//StackPattern k1 = StackPattern(0, 52);
//StackPattern k2 = StackPattern(0, 52);

RandomPattern r1 = RandomPattern(0, 52);
RandomPattern r2 = RandomPattern(0, 52);
RandomPattern r3 = RandomPattern(0, 52);
RandomPattern r4 = RandomPattern(0, 52);
RandomPattern r5 = RandomPattern(0, 52);
RandomPattern r6 = RandomPattern(0, 52);

Pattern* pInnerPatterns[] = {&i1, &i2, 0};
Pattern* pOuterPatterns[] = {
  &f1, &d2, &f2, &d3, &f3, &d1,
  &s4, &d3, &s5, &d2, &s6, &d1,
  &r1, &d2, &r2, &d3, &r3, &d1,
  &f4, &d3, &f5, &d2, &f6, &d1,
  &s1, &d2, &s2, &d3, &s3, &d1,
  &r4, &d3, &r5, &d2, &r6, &d1,
  0,
};

void setup()
{
    sign.setBrightness(NP_BRIGHT ? 240 : 32);
    sign.setMotionPin(8);
    sign.setPixelCount(97);
    sign.setPixelPin(12);
    sign.setSoundInput(0);
    sign.setup();

    /*i1.setPrimaryColor(redMotion);
    i1.setSecondaryColor(whiteMotion);
    i2.setPrimaryColor(whiteMotion);
    i2.setSecondaryColor(redMotion);*/

    d1.setPrimaryColor(BLACK);
    d2.setPrimaryColor(WHITE);
    d3.setPrimaryColor(RED);

    f1.setPrimaryColor(BLACK);
    f1.setSecondaryColor(WHITE);
    f2.setPrimaryColor(WHITE);
    f2.setSecondaryColor(RED);
    f3.setPrimaryColor(RED);
    f3.setSecondaryColor(BLACK);
    f4.setPrimaryColor(BLACK);
    f4.setSecondaryColor(RED);
    f5.setPrimaryColor(RED);
    f5.setSecondaryColor(WHITE);
    f6.setPrimaryColor(WHITE);
    f6.setSecondaryColor(BLACK);

    s1.setPrimaryColor(BLACK);
    s1.setSecondaryColor(WHITE);
    s2.setPrimaryColor(WHITE);
    s2.setSecondaryColor(RED);
    s3.setPrimaryColor(RED);
    s3.setSecondaryColor(BLACK);
    s4.setPrimaryColor(BLACK);
    s4.setSecondaryColor(RED);
    s5.setPrimaryColor(RED);
    s5.setSecondaryColor(WHITE);
    s6.setPrimaryColor(WHITE);
    s6.setSecondaryColor(BLACK);

    s1.setFlags(0x01);
    s2.setFlags(0x01);
    s3.setFlags(0x01);

    //h1.setPrimaryColor(WHITE);
    //h1.setSecondaryColor(RED);

    //k1.setPrimaryColor(WHITE);
    //k1.setSecondaryColor(RED);
    //k2.setPrimaryColor(RED);
    //k2.setSecondaryColor(WHITE);
    //k1.setFlags(0x01);
    //k2.setFlags(0x01);

    r1.setPrimaryColor(BLACK);
    r1.setSecondaryColor(WHITE);
    r2.setPrimaryColor(WHITE);
    r2.setSecondaryColor(RED);
    r3.setPrimaryColor(RED);
    r3.setSecondaryColor(BLACK);
    r4.setPrimaryColor(BLACK);
    r4.setSecondaryColor(RED);
    r5.setPrimaryColor(RED);
    r5.setSecondaryColor(WHITE);
    r6.setPrimaryColor(WHITE);
    r6.setSecondaryColor(BLACK);
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

    if (outerIndex >= 36) {
        outerIndex = 0;
        pOuter = pOuterPatterns[0];
        outerResult = false;
    }
    pOuter->setPixelShift(29);
    if (!outerResult) {
        pOuter->reset();
    }
    outerResult = pOuter->tick(&sign);
    if (!outerResult) {
        outerIndex++;
    }
    
    sign.wait(40);
}

