/*
 * Ansible Sign Blinky Lights - Sign
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

#ifndef SIGN_H
#define SIGN_H

#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "Colors.h"

class Sign {

  public:

    Sign();
    ~Sign();

    void
    setup(),
    setBrightness(uint8_t brightness),
    setMotionPin(int pin),
    setPixelColor(uint16_t pixel, const Color &color),
    setPixelCount(int count),
    setPixelPin(int pin),
    setSoundInput(int input),
    showPixels(),
    wait(int ms);

    int
    getMotionPin() const,
    getPixelCount() const,
    getPixelPin() const,
    getSoundInput() const;

    uint8_t
    getBrightness() const,
    getMotion() const,
    getSound() const;

  protected:

    Adafruit_NeoPixel
    m_pixels;

    int
    m_motionPin,
    m_soundInput;

    uint8_t
    m_lastSound;

};

#endif  // SIGN_H

