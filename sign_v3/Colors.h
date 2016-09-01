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

#ifndef COLORS_H
#define COLORS_H

#include <Arduino.h>
 
class Color {

  public:

    Color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0,
          int8_t motion = 0, int8_t sound = 0);
    Color(const Color &color);
    ~Color();

    void
    darken(uint8_t level),
    applyMotion(uint8_t motion),
    applySound(uint8_t sound);

    //Color operator+(const Color &color);

    uint8_t
    m_red,    // Red component of color.
    m_green,  // Green component of color.
    m_blue;   // Blue component of color.

    int8_t
    m_motion, // Brightness of color influenced by motion.
              // -128 == Full brightness without motion, dark with motion.
              //  -63 == Full brightness without motion, half brightness with motion. 
              //    0 == No change based on motion.
              //   63 == Half brightness without motion, full brighness with motion.
              //  127 == Dark without motion, Full brightness with motion.

    m_sound;  // Brightness of color influenced by sound.
              // -128 == Full brightness without sound, darkens with increased sound.
              //  -63 == Full brightness without sound, half darkens with sound.
              //    0 == No change based on sound.
              //   63 == Half dark without sound, up to full brightness with sound.
              //  127 == Dark without sound, up to full brightness with sound.

};

extern const Color BLACK;
extern const Color WHITE;
extern const Color RED;
extern const Color GREEN;
extern const Color BLUE;
extern const Color MANGO;  // Ansible Red (exact)
extern const Color POOL;   // Ansible Green (exact)
extern const Color ARED;   // Ansible Red (adjusted for sign/wall)
extern const Color AGREEN; // Ansible Green (adjusted)

#endif // COLORS_H

