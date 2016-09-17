/*
 * Ansible Sign Blinky Lights - Sign Regions
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

#include "Region.h"
#include "Sign.h"

Region::Region(Sign *pSign, uint16_t offset, uint16_t count,
               uint8_t shift, uint8_t flags) :
    m_pSign(pSign), m_offset(offset), m_count(count),
    m_shift(shift), m_flags(flags)
{
}

Region::Region(config_region_t &region_t) :
    m_pSign(0), m_offset(region_t.offset), m_count(region_t.count),
    m_shift(region_t.shift), m_flags(region_t.flags)
{
}

Region::~Region()
{  
}

void
Region::setSign(Sign *pSign)
{
    m_pSign = pSign;
}

void
Region::setOffset(uint16_t offset)
{
    m_offset = offset;
}

void
Region::setCount(uint16_t count)
{
    m_count = count; 
}

void
Region::setShift(uint8_t shift)
{
    m_shift = shift;
}

void
Region::setFlags(uint8_t flags)
{
    m_flags = flags;
}

void
Region::setRelativePixel(uint16_t pixel, const Color &color)
{
    if (!m_pSign) {
        return;
    }

    if (m_shift) {
        pixel = (pixel + m_shift) % m_count;
    }

    if (m_flags & 0x01) { // reverse
        pixel = m_count - pixel - 1;
    }

    pixel += m_offset;
    if (pixel < m_pSign->getPixelCount()) {
        m_pSign->setPixelColor(pixel, color);
    }
}

void
Region::setAllPixels(const Color &color)
{
    if (m_pSign) {
        for (uint16_t pixel = 0; pixel < m_count; pixel++) {
            setRelativePixel(pixel, color);
        }
    }
}

void
Region::showPixels()
{
    if (m_pSign) {
        m_pSign->showPixels();
    }
}

Sign *
Region::getSign()
{
    return m_pSign;
}

uint16_t
Region::getOffset()
{
    return m_offset;
}

uint16_t
Region::getCount()
{
    return m_count;
}

uint8_t
Region::getShift()
{
    return m_shift;
}

uint8_t
Region::getFlags()
{
    return m_flags;
}

