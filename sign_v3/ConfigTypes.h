/*
 * Ansible Sign Blinky Lights - Config Types
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

#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <Arduino.h>

typedef enum {
  SOLID = 0,
  FADE
} pattern_type_t;

// 0x10
typedef struct {
  uint8_t pixelOutput;
  uint8_t motionInput;
  uint8_t soundInput;
  uint8_t reserved[3];
} config_pins_t;

// 0x11
typedef struct {
  uint16_t pixelCount;
  uint8_t regionCount;
  uint8_t waitms;
  uint8_t reserved[2];
} config_options_t;

// 0x20 - 0x2f
typedef struct {
  uint16_t offset;
  uint16_t count;
  uint8_t shift;
  uint8_t flags;
} config_region_t;

// 0x40 - 0x7f
typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  int8_t motion;
  int8_t sound;
  uint8_t reserved;
} config_color_t;

// 0x80 - 0xbf
typedef struct {
  uint8_t patternType;
  uint8_t primaryColorIndex;
  uint8_t secondaryColorIndex;
  uint8_t tertiaryColorIndex;
  uint8_t flags;
  uint8_t reserved;
} config_pattern_t;

// 0xc0 - 0xef
typedef struct {
  uint8_t regionIndex;
  uint8_t patternIndex;
  uint8_t nextSequenceIndex;
  uint8_t reserved[3];
} config_sequence_t;

typedef union {
  config_pins_t pins;
  config_options_t options;
  config_region_t region;
  config_color_t color;
  config_pattern_t pattern;
  config_sequence_t sequence;
} config_data_t;

typedef struct {
  uint8_t id;
  config_data_t data;
  uint8_t crc8;
} config_slot_t;


// 1KB EEPROM
// 128 x 8 byte slots - 1 byte id, 6 bytes data, 1 byte crc8
// 0x00-0x3f - reserved for now
// 0x40-0x7f - colors
// 0x80-0xbf - inner patterns
// 0xc0-0xfe - outer patterns
// 0xff - free

#define CONFIG_SLOT_NOT_FOUND 0xff

#endif  // CONFIG_TYPES_H

