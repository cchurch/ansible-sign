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
#include "ConfigTypes.h"
#include "Colors.h"
#include "Region.h"

class Pattern {

  public:

    Pattern(Region *pRegion = 0,
            const Color &primaryColor = BLACK,
            const Color &secondaryColor = BLACK,
            const Color &tertiaryColor = BLACK,
            uint8_t flags = 0);
    virtual ~Pattern();

    virtual void reset();
    virtual bool update();

    void
    setRegion(Region *pRegion),
    setPrimaryColor(const Color &color),
    setSecondaryColor(const Color &color),
    setTertiaryColor(const Color &color),
    setFlags(uint8_t flags);

    Region *
    getRegion();

    uint8_t
    getFlags();

    uint16_t
    getPixelCount();

  protected:

    void
    setRelativePixel(uint16_t pixel, const Color &color);

    Region *
    m_pRegion;

    uint8_t
    m_flags;

    Color
    m_primaryColor,
    m_secondaryColor,
    m_tertiaryColor;

};

class SolidPattern : public Pattern {

  public:

    SolidPattern(Region *pRegion = 0,
                 const Color &primaryColor = BLACK,
                 const Color &secondaryColor = BLACK,
                 const Color &tertiaryColor = BLACK,
                 uint8_t flags = 0);
    virtual ~SolidPattern();

    virtual void reset();
    virtual bool update();

  protected:

    uint8_t
    m_index;

};

class FadePattern : public Pattern {

  public:

    FadePattern(Region *pRegion = 0,
                const Color &primaryColor = BLACK,
                const Color &secondaryColor = BLACK,
                const Color &tertiaryColor = BLACK,
                uint8_t flags = 0);
    virtual ~FadePattern();

    virtual void reset();
    virtual bool update();

  protected:

    uint8_t
    mixValues(uint8_t v1, uint8_t v2);

    uint8_t
    m_fadeIndex;

};

class SpinPattern : public Pattern {

  public:

    SpinPattern(Region *pRegion = 0,
                const Color &primaryColor = BLACK,
                const Color &secondaryColor = BLACK,
                const Color &tertiaryColor = BLACK,
                uint8_t flags = 0);
    virtual ~SpinPattern();

    virtual void reset();
    virtual bool update();

  protected:

    uint16_t
    m_spinIndex;

};

class StackPattern : public Pattern {

  public:

    StackPattern(Region *pRegion = 0,
                 const Color &primaryColor = BLACK,
                 const Color &secondaryColor = BLACK,
                 const Color &tertiaryColor = BLACK,
                 uint8_t flags = 0);
    virtual ~StackPattern();

    virtual void reset();
    virtual bool update();

  protected:

    uint16_t
    m_currentIndex,
    m_stackIndex;

};

class HalvesPattern : public Pattern {

  public:

    HalvesPattern(Region *pRegion = 0,
                  const Color &primaryColor = BLACK,
                  const Color &secondaryColor = BLACK,
                  const Color &tertiaryColor = BLACK,
                  uint8_t flags = 0);
    virtual ~HalvesPattern();

    virtual void reset();
    virtual bool update();

  protected:

    uint16_t
    m_splitIndex;

};

class RandomPattern : public Pattern {

  public:

    RandomPattern(Region *pRegion = 0,
                  const Color &primaryColor = BLACK,
                  const Color &secondaryColor = BLACK,
                  const Color &tertiaryColor = BLACK,
                  uint8_t flags = 0);
    virtual ~RandomPattern();

    virtual void reset();
    virtual bool update();

  protected:

    static const uint8_t m_bitsLength = 8;
    uint8_t
    m_bits[m_bitsLength];

};

class FirePattern : public Pattern {

  public:

    FirePattern(Region *pRegion = 0,
                 const Color &primaryColor = BLACK,
                 const Color &secondaryColor = BLACK,
                 const Color &tertiaryColor = BLACK,
                 uint8_t flags = 0);
    virtual ~FirePattern();

    virtual void reset();
    virtual bool update();

  protected:

    uint8_t
    m_index;

};

#endif  // PATTERNS_H

