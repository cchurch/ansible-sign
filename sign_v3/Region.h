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

#ifndef REGION_H
#define REGION_H

#include <Arduino.h>
#include "ConfigTypes.h"
#include "Colors.h"

class Sign;

class Region {

  public:

    Region(Sign *pSign = 0,
           uint16_t offset = 0,
           uint16_t count = 0,
           uint8_t shift = 0,
           uint8_t flags = 0);
    Region(config_region_t &region_t);
    ~Region();

    void
    setSign(Sign *pSign),
    setOffset(uint16_t offset),
    setCount(uint16_t count),
    setShift(uint8_t shift),
    setFlags(uint8_t flags),
    setRelativePixel(uint16_t pixel, const Color &color),
    setAllPixels(const Color &color),
    showPixels();

    Sign *
    getSign();

    uint16_t
    getOffset(),
    getCount();

    uint8_t
    getShift(),
    getFlags();

  protected:

    Sign *
    m_pSign;
 
    uint16_t
    m_offset,
    m_count;

    uint8_t
    m_shift,
    m_flags;
    
};

#endif  // REGION_H

