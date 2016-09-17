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

#include "Patterns.h"

Pattern::Pattern(Region *pRegion,
                 const Color &primaryColor,
                 const Color &secondaryColor,
                 const Color &tertiaryColor,
                 uint8_t flags) :
    m_pRegion(pRegion),
    m_primaryColor(primaryColor),
    m_secondaryColor(secondaryColor),
    m_tertiaryColor(tertiaryColor),
    m_flags(flags)
{
}

Pattern::~Pattern()
{
}

void
Pattern::reset()
{
}

bool
Pattern::update()
{
    return false;
}

void
Pattern::setPrimaryColor(const Color &color)
{
    m_primaryColor = color;
}

void
Pattern::setSecondaryColor(const Color &color)
{
    m_secondaryColor = color;
}

void
Pattern::setTertiaryColor(const Color &color)
{
    m_tertiaryColor = color;
}

void
Pattern::setFlags(uint8_t flags)
{
    m_flags = flags;
}

uint16_t
Pattern::getPixelCount()
{
    if (m_pRegion) {
        return m_pRegion->getCount();
    }
    else {
        return 0;
    }
}

void
Pattern::setRelativePixel(uint16_t pixel, const Color &color)
{
    uint16_t pixelCount = getPixelCount();

    if (m_pRegion && pixelCount) {
        if (m_flags & 0x01) { // reverse
            pixel = pixelCount - pixel - 1;
        }
        m_pRegion->setRelativePixel(pixel, color);
    }
}
    
SolidPattern::SolidPattern(Region *pRegion,
                           const Color &primaryColor,
                           const Color &secondaryColor,
                           const Color &tertiaryColor,
                           uint8_t flags) :
    Pattern(pRegion, primaryColor, secondaryColor, tertiaryColor, flags),
    m_index(0)
{
}

SolidPattern::~SolidPattern()
{
}

void
SolidPattern::reset()
{
    m_index = 0;
}

bool
SolidPattern::update()
{
    if (m_index >= getPixelCount() || !m_pRegion) {
        return false;
    }

    m_pRegion->setAllPixels(m_primaryColor);
    m_pRegion->showPixels();

    m_index++;
    return true;
}

FadePattern::FadePattern(Region *pRegion,
                         const Color &primaryColor,
                         const Color &secondaryColor,
                         const Color &tertiaryColor,
                         uint8_t flags) :
    Pattern(pRegion, primaryColor, secondaryColor, tertiaryColor, flags),
    m_fadeIndex(0)
{
}

FadePattern::~FadePattern()
{
}

void
FadePattern::reset()
{
    m_fadeIndex = 0;
}

uint8_t
FadePattern::mixValues(uint8_t v1, uint8_t v2)
{
    uint16_t pixelCount = getPixelCount();

    if (!pixelCount) {
        return v1;
    }
    return (v1 * (pixelCount - m_fadeIndex) + v2 * m_fadeIndex) / pixelCount;
}

bool
FadePattern::update()
{
    Color mixedColor = BLACK;

    if (m_fadeIndex >= getPixelCount() || !m_pRegion) {
        return false;
    }

    mixedColor.m_red = mixValues(m_primaryColor.m_red, m_secondaryColor.m_red);
    mixedColor.m_green = mixValues(m_primaryColor.m_green, m_secondaryColor.m_green);
    mixedColor.m_blue = mixValues(m_primaryColor.m_blue, m_secondaryColor.m_blue);
    mixedColor.m_motion = mixValues(m_primaryColor.m_motion, m_secondaryColor.m_motion);
    mixedColor.m_sound = mixValues(m_primaryColor.m_sound, m_secondaryColor.m_sound);
    m_pRegion->setAllPixels(mixedColor);
    m_pRegion->showPixels();

    m_fadeIndex++;
    return true;
}

SpinPattern::SpinPattern(Region *pRegion,
                         const Color &primaryColor,
                         const Color &secondaryColor,
                         const Color &tertiaryColor,
                         uint8_t flags) :
    Pattern(pRegion, primaryColor, secondaryColor, tertiaryColor, flags),
    m_spinIndex(0)
{
}

SpinPattern::~SpinPattern()
{
}

void
SpinPattern::reset()
{
    m_spinIndex = 0;
}

bool
SpinPattern::update()
{
    uint16_t pixelCount = getPixelCount();

    if (m_spinIndex >= pixelCount || !m_pRegion) {
        return false;
    }

    for (uint16_t p = 0; p < pixelCount; p++) {
        if (p <= m_spinIndex) {
            setRelativePixel(p, m_secondaryColor);
        }
        else {
            setRelativePixel(p, m_primaryColor);
        }
    }
    m_pRegion->showPixels();

    m_spinIndex++;
    return true;
}

StackPattern::StackPattern(Region *pRegion,
                           const Color &primaryColor,
                           const Color &secondaryColor,
                           const Color &tertiaryColor,
                           uint8_t flags) :
    Pattern(pRegion, primaryColor, secondaryColor, tertiaryColor, flags),
    m_currentIndex(0), m_stackIndex(0)
{
}

StackPattern::~StackPattern()
{
}

void
StackPattern::reset()
{
    m_currentIndex = m_stackIndex = 0;
}

bool
StackPattern::update()
{
    uint16_t pixelCount = getPixelCount();

    if (m_stackIndex >= pixelCount || !m_pRegion) {
        return false;
    }

    for (uint16_t p = 0; p < pixelCount; p++) {
        if (p == m_currentIndex || p > (pixelCount - m_stackIndex)) {
            setRelativePixel(p, m_secondaryColor);
        }
        else {
            setRelativePixel(p, m_primaryColor);
        }
    }
    m_pRegion->showPixels();

    if (++m_currentIndex >= (pixelCount - m_stackIndex)) {
        m_stackIndex++;
        m_currentIndex = 0;
    }

    return true;
}

HalvesPattern::HalvesPattern(Region *pRegion,
                             const Color &primaryColor,
                             const Color &secondaryColor,
                             const Color &tertiaryColor,
                             uint8_t flags) :
    Pattern(pRegion, primaryColor, secondaryColor, tertiaryColor, flags),
    m_splitIndex(0)
{
}

HalvesPattern::~HalvesPattern()
{
}

void
HalvesPattern::reset()
{
    m_splitIndex = 0;
}

bool
HalvesPattern::update()
{
    uint16_t pixelCount = getPixelCount();

    if (m_splitIndex >= pixelCount || !m_pRegion) {
        return false;
    }

    for (uint16_t p = 0; p < pixelCount; p++) {
        if (((p + m_splitIndex) % pixelCount) < (pixelCount / 2)) {
            setRelativePixel(p, m_secondaryColor);
        }
        else {
            setRelativePixel(p, m_primaryColor);
        }
    }
    m_pRegion->showPixels();

    m_splitIndex++;
    return true;
}

RandomPattern::RandomPattern(Region *pRegion,
                             const Color &primaryColor,
                             const Color &secondaryColor,
                             const Color &tertiaryColor,
                             uint8_t flags) :
    Pattern(pRegion, primaryColor, secondaryColor, tertiaryColor, flags)
{
    reset();
}

RandomPattern::~RandomPattern()
{
}

void
RandomPattern::reset()
{
    for (uint8_t i = 0; i < m_bitsLength; i++) {
        m_bits[i] = 0;
    }
}

bool
RandomPattern::update()
{
    uint16_t pixelCount = getPixelCount();
    uint8_t hasUnsetBits = 0;
    uint16_t p = 0;

    for (p = 0; p < pixelCount; p++) {
        if ((p / 8) >= m_bitsLength) {
            continue;
        }
        if (bitRead(m_bits[p / 8],  (p % 8))) {
            continue;
        }
        hasUnsetBits = 1;
        break;
    }
    if (!hasUnsetBits || !m_pRegion) {
        return false;
    }

    while (1) {
        p = random(pixelCount);
        if ((p / 8) >= m_bitsLength) {
            continue;
        }
        if (bitRead(m_bits[p / 8],  (p % 8))) {
            continue;
        }
        bitSet(m_bits[p / 8], (p % 8));
        break;
    }

    for (p = 0; p < pixelCount; p++) {
        if ((p / 8) >= m_bitsLength) {
            continue;
        }
        if (bitRead(m_bits[p / 8],  (p % 8))) {
            setRelativePixel(p, m_secondaryColor);
        }
        else {
            setRelativePixel(p, m_primaryColor);
        }
    }
    m_pRegion->showPixels();

    return true;
}

