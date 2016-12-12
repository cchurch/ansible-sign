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
#include "Sign.h"
#include "Region.h"
#include "Patterns.h"
#include "Remote.h"

// Run in bright mode (set to 0 when working up close to avoid blinding yourself).
#define NP_BRIGHT 1

// Outside ring, clockwise.
#define NP_OUTER 52

// Inside ring, counterclockwise.
#define NP_INNER 45

Color innerWhite = Color(255, 255, 255, 64, 64);
Color innerRed = Color(255, 0, 0, 64, 64);
Color black = Color(0, 0, 0, 32, 32);
Color white = Color(255, 255, 255, 32, 32);
Color red = Color(255, 0, 0, 32, 32);
Color green = Color(0, 255, 0, 32, 32);
Color blue = Color(0, 0, 255, 32, 32);
Color yellow = Color(255, 255, 0, 32, 32);
Color cyan = Color(0, 255, 255, 32, 32);
Color purple = Color(255, 0, 255, 32, 32);
Color orange = Color(255, 127, 0, 32, 32);

Color fire1 = Color(255, 31, 0, -16, -32);
Color fire2 = Color(255, 63, 0, -32, -16);
Color fire3 = Color(255, 95, 0, 16, 16);

//const Color ARED =  Color(0xff, 0x88, 0x80);   // Ansible Red (adjusted for sign/wall)
//const Color AGREEN = Color(0x24, 0xc5, 0x57); // Ansible Green (adjusted)


Remote remote = Remote();

Sign sign = Sign(97, 12, 8, 0);

Region inner = Region(&sign, 52, 45);
Region outer = Region(&sign, 0, 52, 29);

FadePattern i1 = FadePattern(&inner, innerRed, innerWhite);
FadePattern i2 = FadePattern(&inner, innerWhite, innerRed);

SolidPattern sBlack = SolidPattern(&outer, black);

//SolidPattern d1 = SolidPattern(&outer, black);
//SolidPattern d2 = SolidPattern(&outer, white);
//SolidPattern d3 = SolidPattern(&outer, red);

//FadePattern f1 = FadePattern(&outer, black, white);
//FadePattern f2 = FadePattern(&outer, white, red);
//FadePattern f3 = FadePattern(&outer, red, black);
//FadePattern f4 = FadePattern(&outer, black, red);
//FadePattern f5 = FadePattern(&outer, red, white);
//FadePattern f6 = FadePattern(&outer, white, black);

FadePattern fBlackToRed(&outer, black, red);
FadePattern fRedToGreen(&outer, red, green);
FadePattern fGreenToBlue(&outer, green, blue);
FadePattern fBlueToYellow(&outer, blue, yellow);
FadePattern fYellowToCyan(&outer, yellow, cyan);
FadePattern fCyanToPurple(&outer, cyan, purple);
FadePattern fPurpleToWhite(&outer, purple, white);
FadePattern fWhiteToBlack(&outer, white, black);

RandomPattern rBlackToRed(&outer, black, red);
RandomPattern rRedToGreen(&outer, red, green);
RandomPattern rGreenToBlue(&outer, green, blue);
RandomPattern rBlueToYellow(&outer, blue, yellow);
RandomPattern rYellowToCyan(&outer, yellow, cyan);
RandomPattern rCyanToPurple(&outer, cyan, purple);
RandomPattern rPurpleToWhite(&outer, purple, white);
RandomPattern rWhiteToBlack(&outer, white, black);

//SpinPattern s1 = SpinPattern(&outer, black, white, black, 1);
//SpinPattern s2 = SpinPattern(&outer, white, red, black, 1);
//SpinPattern s3 = SpinPattern(&outer, red, black, black, 1);
//SpinPattern s4 = SpinPattern(&outer, black, red);
//SpinPattern s5 = SpinPattern(&outer, red, white);
//SpinPattern s6 = SpinPattern(&outer, white, black);

//HalvesPattern h1 = HalvesPattern(&outer, white, red);

//StackPattern k1 = StackPattern(&outer, white, red, black, 1);
//StackPattern k2 = StackPattern(&outer, red, white, black, 1);

//RandomPattern r1 = RandomPattern(&outer, black, white);
//RandomPattern r2 = RandomPattern(&outer, white, red);
//RandomPattern r3 = RandomPattern(&outer, red, black);
//RandomPattern r4 = RandomPattern(&outer, black, red);
//RandomPattern r5 = RandomPattern(&outer, red, white);
//RandomPattern r6 = RandomPattern(&outer, white, black);

FirePattern pFire1 = FirePattern(&outer, fire1, fire2, fire3, 0x04);
FirePattern pFire2 = FirePattern(&outer, fire1, fire2, fire3, 0x05);
FirePattern pFire3 = FirePattern(&outer, fire1, fire2, fire3, 0x06);

FirePattern pFire4 = FirePattern(&outer, black, white, red, 0x00);
FirePattern pFire5 = FirePattern(&outer, black, white, red, 0x01);
FirePattern pFire6 = FirePattern(&outer, black, white, red, 0x02);

FirePattern pFire7 = FirePattern(&outer, red, green, white, 0x00);
FirePattern pFire8 = FirePattern(&outer, red, green, white, 0x01);
FirePattern pFire9 = FirePattern(&outer, red, green, white, 0x02);

FirePattern pFire10 = FirePattern(&outer, green, white, black, 0x00);
FirePattern pFire11 = FirePattern(&outer, green, white, black, 0x01);
FirePattern pFire12 = FirePattern(&outer, green, white, black, 0x02);

Pattern* pInnerPatterns[] = {
  &i1, &i2,
  0,
};

Pattern* pOuterPatterns[] = {
  &fBlackToRed, &fRedToGreen, &fGreenToBlue, &fBlueToYellow, &fYellowToCyan, &fCyanToPurple, &fPurpleToWhite, &fWhiteToBlack,
  &sBlack,
  &pFire1, &pFire2, &pFire3, &pFire1, &pFire2, &pFire3, &pFire1, &pFire2, &pFire3, &pFire1, &pFire2, &pFire3,
  &sBlack,
  &rBlackToRed, &rRedToGreen, &rGreenToBlue, &rBlueToYellow, &rYellowToCyan, &rCyanToPurple, &rPurpleToWhite, &rWhiteToBlack,
  &sBlack,
  &pFire4, &pFire5, &pFire6, &pFire7, &pFire8, &pFire9, &pFire10, &pFire11, &pFire12,
  &sBlack,


  //&f1, &d2, &f2, &d3, &f3, &d1,
  //&s4, &d3, &s5, &d2, &s6, &d1,
  //&r1, &d2, &r2, &d3, &r3, &d1,
  //&f4, &d3, &f5, &d2, &f6, &d1,
  //&s1, &d2, &s2, &d3, &s3, &d1,
  //&r4, &d3, &r5, &d2, &r6, &d1,
  0,
};

void setup()
{
    remote.setup();
    sign.setBrightness(NP_BRIGHT ? 240 : 32);
    sign.setup();
}

void loop()
{
    static int innerIndex = 0;
    static int outerIndex = 0;
    static bool innerResult = false;
    static bool outerResult = false;

    Pattern *pInner = pInnerPatterns[innerIndex];
    Pattern *pOuter = pOuterPatterns[outerIndex];

    if (!pInner) {
        innerIndex = 0;
        pInner = pInnerPatterns[0];
        innerResult = false;
    }
    if (!innerResult) {
        pInner->reset();
    }
    innerResult = pInner->update();
    if (!innerResult) {
        innerIndex++;
    }

    if (!pOuter) {
        outerIndex = 0;
        pOuter = pOuterPatterns[0];
        outerResult = false;
    }
    if (!outerResult) {
        pOuter->reset();
    }
    outerResult = pOuter->update();
    if (!outerResult) {
        outerIndex++;
    }

    remote.loop();
    sign.wait(40);
}

