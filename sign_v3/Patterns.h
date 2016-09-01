/*
 * Ansible Sign Blinky Lights - Patterns
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

#ifndef PATTERNS_H
#define PATTERNS_H

#include <Arduino.h>
#include "Colors.h"
#include "Sign.h"

class Pattern {

  public:

    Pattern(uint16_t offset = 0, uint16_t count = 0,
            const Color &primaryColor = BLACK,
            const Color &secondaryColor = BLACK,
            const Color &tertiaryColor = BLACK);
    virtual ~Pattern();

    virtual void reset();
    virtual bool tick(Sign *pSign);

    void
    setPixelOffset(uint16_t offset),
    setPixelCount(uint16_t count),
    setPrimaryColor(const Color &color),
    setSecondaryColor(const Color &color),
    setTertiaryColor(const Color &color);

    uint16_t
    getPixelCount();

  protected:

    void updateAll(Sign *pSign, const Color &color);

    uint16_t m_pixelOffset;
    uint16_t m_pixelCount;

    Color
    m_primaryColor,
    m_secondaryColor,
    m_tertiaryColor;

};

class SolidPattern : public Pattern {

  public:

    SolidPattern(uint16_t offset = 0, uint16_t count = 0,
                 const Color &primaryColor = BLACK,
                 const Color &secondaryColor = BLACK,
                 const Color &tertiaryColor = BLACK);
    virtual ~SolidPattern();

    virtual void reset();
    virtual bool tick(Sign *pSign);

  protected:

    uint8_t
    m_currentColorIndex;

};

class FadePattern : public Pattern {

  public:

    FadePattern(uint16_t offset = 0, uint16_t count = 0,
                 const Color &primaryColor = BLACK,
                 const Color &secondaryColor = BLACK,
                 const Color &tertiaryColor = BLACK);
    virtual ~FadePattern();

    virtual void reset();
    virtual bool tick(Sign *pSign);

  protected:

    uint8_t
    mixValues(uint8_t v1, uint8_t v2);

    uint8_t
    m_fadeIndex;

};


#endif  // PATTERNS_H

