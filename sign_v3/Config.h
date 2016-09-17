/*
 * Ansible Sign Blinky Lights - Config
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

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "ConfigTypes.h"

class Config {

  public:

    Config();
    ~Config();

    static uint8_t crc8(const uint8_t *data, uint8_t len);

    bool
    readSlot(uint8_t slotIndex, config_slot_t &slotData),
    writeSlot(uint8_t slotIndex, config_slot_t &slotData),
    freeSlot(uint8_t slotIndex);

    uint8_t findFirstSlot(uint8_t startId = 0, uint8_t endId = 0);
    uint8_t findFreeSlot(uint8_t startId = 0, uint8_t endId = 0);

  protected:

};

#endif  // CONFIG_H

