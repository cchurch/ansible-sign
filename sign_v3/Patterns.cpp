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

Pattern::Pattern(uint16_t offset, uint16_t count,
                 const Color primaryColor,
                 const Color secondaryColor,
                 const Color tertiaryColor) :
    m_flags(0), m_pixelOffset(offset), m_pixelCount(count), m_pixelShift(0),
    m_primaryColor(primaryColor),
    m_secondaryColor(secondaryColor),
    m_tertiaryColor(tertiaryColor)
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
Pattern::tick(Sign *pSign)
{
    return false;
}

void
Pattern::setRelativePixelColor(Sign *pSign, uint16_t pixel, const Color &color)
{
    if (pSign) {
        if (m_pixelShift) {
            pixel = (pixel + m_pixelShift) % m_pixelCount;
        }
        if (m_flags & 0x01) { // reverse
            pixel = m_pixelCount - pixel - 1;
        }
        pixel += m_pixelOffset;
        if (pixel < pSign->getPixelCount()) {
            pSign->setPixelColor(pixel, color);
        }
    }
}

void
Pattern::updateAll(Sign *pSign, const Color &color)
{
    if (pSign) {
        for (uint16_t p = 0; p < m_pixelCount; p++) {
            setRelativePixelColor(pSign, p, color);
        }
        pSign->showPixels();
    }
}

void
Pattern::setFlags(uint8_t flags)
{
    m_flags = flags;
}

void
Pattern::setPixelOffset(uint16_t offset)
{
    m_pixelOffset = offset;
}

void
Pattern::setPixelCount(uint16_t count)
{
    m_pixelCount = count;
}

void
Pattern::setPixelShift(uint16_t shift)
{
    m_pixelShift = shift;
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

uint16_t
Pattern::getPixelCount()
{
    return m_pixelCount;
}

SolidPattern::SolidPattern(uint16_t offset, uint16_t count,
                           const Color primaryColor,
                           const Color secondaryColor,
                           const Color tertiaryColor) :
    Pattern(offset, count, primaryColor, secondaryColor, tertiaryColor),
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
SolidPattern::tick(Sign *pSign)
{
    if (m_index >= m_pixelCount || !pSign) {
        return false;
    }

    updateAll(pSign, m_primaryColor);

    m_index++;
    return true;
}

FadePattern::FadePattern(uint16_t offset, uint16_t count,
                         const Color primaryColor,
                         const Color secondaryColor,
                         const Color tertiaryColor) :
    Pattern(offset, count, primaryColor, secondaryColor, tertiaryColor),
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
    if (!m_pixelCount) {
        return v1;
    }
    return (v1 * (m_pixelCount - m_fadeIndex) + v2 * m_fadeIndex) / m_pixelCount;
}

bool
FadePattern::tick(Sign *pSign)
{
    Color mixedColor = BLACK;

    if (m_fadeIndex >= m_pixelCount) {
        return false;
    }

    mixedColor.m_red = mixValues(m_primaryColor.m_red, m_secondaryColor.m_red);
    mixedColor.m_green = mixValues(m_primaryColor.m_green, m_secondaryColor.m_green);
    mixedColor.m_blue = mixValues(m_primaryColor.m_blue, m_secondaryColor.m_blue);
    mixedColor.m_motion = mixValues(m_primaryColor.m_motion, m_secondaryColor.m_motion);
    mixedColor.m_sound = mixValues(m_primaryColor.m_sound, m_secondaryColor.m_sound);
    updateAll(pSign, mixedColor);

    m_fadeIndex++;
    return true;
}

SpinPattern::SpinPattern(uint16_t offset, uint16_t count,
                         const Color primaryColor,
                         const Color secondaryColor,
                         const Color tertiaryColor) :
    Pattern(offset, count, primaryColor, secondaryColor, tertiaryColor),
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
SpinPattern::tick(Sign *pSign)
{
    if (m_spinIndex >= m_pixelCount) {
        return false;
    }

    if (pSign) {
        for (uint16_t p = 0; p < m_pixelCount; p++) {
            if (p <= m_spinIndex) {
                setRelativePixelColor(pSign, p, m_secondaryColor);
            }
            else {
                setRelativePixelColor(pSign, p, m_primaryColor);
            }
        }
        pSign->showPixels();
    }

    m_spinIndex++;
    return true;
}

StackPattern::StackPattern(uint16_t offset, uint16_t count,
                         const Color primaryColor,
                         const Color secondaryColor,
                         const Color tertiaryColor) :
    Pattern(offset, count, primaryColor, secondaryColor, tertiaryColor),
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
StackPattern::tick(Sign *pSign)
{
    if (m_stackIndex >= m_pixelCount || !pSign) {
        return false;
    }

    for (uint16_t p = 0; p < m_pixelCount; p++) {
        if (p == m_currentIndex || p > (m_pixelCount - m_stackIndex)) {
            setRelativePixelColor(pSign, p, m_secondaryColor);
        }
        else {
            setRelativePixelColor(pSign, p, m_primaryColor);
        }
    }
    pSign->showPixels();

    if (++m_currentIndex >= (m_pixelCount - m_stackIndex)) {
        m_stackIndex++;
        m_currentIndex = 0;
    }

    return true;
}

HalvesPattern::HalvesPattern(uint16_t offset, uint16_t count,
                         const Color primaryColor,
                         const Color secondaryColor,
                         const Color tertiaryColor) :
    Pattern(offset, count, primaryColor, secondaryColor, tertiaryColor),
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
HalvesPattern::tick(Sign *pSign)
{
    if (m_splitIndex >= m_pixelCount) {
        return false;
    }

    if (pSign) {
        for (uint16_t p = 0; p < m_pixelCount; p++) {
            if (((p + m_splitIndex) % m_pixelCount) < (m_pixelCount / 2)) {
                setRelativePixelColor(pSign, p, m_secondaryColor);
            }
            else {
                setRelativePixelColor(pSign, p, m_primaryColor);
            }
        }
        pSign->showPixels();
    }

    m_splitIndex++;
    return true;
}

RandomPattern::RandomPattern(uint16_t offset, uint16_t count,
                         const Color primaryColor,
                         const Color secondaryColor,
                         const Color tertiaryColor) :
    Pattern(offset, count, primaryColor, secondaryColor, tertiaryColor)
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
RandomPattern::tick(Sign *pSign)
{
    uint8_t hasUnsetBits = 0;
    uint16_t p = 0;

    for (p = 0; p < m_pixelCount; p++) {
        if ((p / 8) >= m_bitsLength) {
            continue;
        }
        if (bitRead(m_bits[p / 8],  (p % 8))) {
            continue;
        }
        hasUnsetBits = 1;
        break;
    }
    if (!hasUnsetBits || !pSign) {
        return false;
    }

    while (1) {
        p = random(m_pixelCount);
        if ((p / 8) >= m_bitsLength) {
            continue;
        }
        if (bitRead(m_bits[p / 8],  (p % 8))) {
            continue;
        }
        bitSet(m_bits[p / 8], (p % 8));
        break;
    }

    for (p = 0; p < m_pixelCount; p++) {
        if ((p / 8) >= m_bitsLength) {
            continue;
        }
        if (bitRead(m_bits[p / 8],  (p % 8))) {
            setRelativePixelColor(pSign, p, m_secondaryColor);
        }
        else {
            setRelativePixelColor(pSign, p, m_primaryColor);
        }
    }
    pSign->showPixels();

    return true;
}


