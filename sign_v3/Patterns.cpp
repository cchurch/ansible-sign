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
                 const Color &primaryColor,
                 const Color &secondaryColor,
                 const Color &tertiaryColor) :
    m_pixelOffset(offset), m_pixelCount(count),
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
Pattern::updateAll(Sign *pSign, const Color &color)
{
    if (pSign) {
        for (uint16_t p = m_pixelOffset; p < (m_pixelOffset + m_pixelCount); p++) {
            pSign->setPixelColor(p, color);
        }
        pSign->showPixels();
    }
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
                           const Color &primaryColor,
                           const Color &secondaryColor,
                           const Color &tertiaryColor) :
    Pattern(offset, count, primaryColor, secondaryColor, tertiaryColor),
    m_currentColorIndex(0)
{
}

SolidPattern::~SolidPattern()
{
}

void
SolidPattern::reset()
{
    m_currentColorIndex = 0;
}

bool
SolidPattern::tick(Sign *pSign)
{
    switch (m_currentColorIndex++) {
        case 0:
            updateAll(pSign, m_primaryColor);
            break;
        case 1:
            updateAll(pSign, m_secondaryColor);
            break;
        case 2:
            updateAll(pSign, m_tertiaryColor);
        default:
            return false;
    }
    return true;
}

FadePattern::FadePattern(uint16_t offset, uint16_t count,
                         const Color &primaryColor,
                         const Color &secondaryColor,
                         const Color &tertiaryColor) :
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
    return (v1 * (256 - m_fadeIndex) + v2 * m_fadeIndex) / 256;
}

bool
FadePattern::tick(Sign *pSign)
{
    Color mixedColor = BLACK;

    if (m_fadeIndex >= 255) {
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


