/*
 * Ansible Sign Blinky Lights - Colors
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

Color::Color(uint8_t red, uint8_t green, uint8_t blue,
             int8_t motion, int8_t sound) :
    m_red(red), m_blue(blue), m_green(green),
    m_motion(motion), m_sound(sound)
{
}

Color::Color(const Color &color) :
    m_red(color.m_red), m_blue(color.m_blue), m_green(color.m_green),
    m_motion(color.m_motion), m_sound(color.m_sound)
{
}

Color::~Color()
{
}

void
Color::darken(uint8_t amount)
{
    uint16_t a = 0x100 - amount;
    m_red = m_red * a / 0x100;
    m_green = m_green * a / 0x100;
    m_blue = m_blue * a / 0x100;
}

void
Color::applyMotion(uint8_t motion)
{
    if (m_motion > 0 && !motion) {
        darken(2 * m_motion + 1);      
    }
    else if (m_motion < 0 && motion) {
        darken(-2 * m_motion - 1);
    }
}

void
Color::applySound(uint8_t sound)
{
    if (m_sound > 0) {
        
    }
    else if (m_sound < 0) {
        
    }
}

const Color BLACK = Color();
const Color WHITE = Color(0xff, 0xff, 0xff);
const Color RED = Color(0xff, 0, 0);
const Color GREEN = Color(0, 0xff, 0);
const Color BLUE = Color(0, 0, 0xff);
const Color MANGO = Color(0xff, 0x58, 0x50);  // Ansible Red (exact)
const Color POOL = Color(0x5b, 0xbd, 0xdf);   // Ansible Green (exact)
const Color ARED =  Color(0xff, 0x88, 0x80);   // Ansible Red (adjusted for sign/wall)
const Color AGREEN = Color(0x24, 0xc5, 0x57); // Ansible Green (adjusted)

