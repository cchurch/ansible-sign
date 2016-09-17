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

#include <EEPROM.h>
#include "Config.h"

Config::Config()
{
}

Config::~Config()
{
}

// From: http://www.leonardomiliani.com/en/2013/un-semplice-crc8-per-arduino/
// CRC-8 - based on the CRC8 formulas by Dallas/Maxim
// code released under the therms of the GNU GPL 3.0 license
uint8_t
Config::crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0x00;
    while (len--) {
        uint8_t extract = *data++;
        for (uint8_t tempI = 8; tempI; tempI--) {
            uint8_t sum = (crc ^ extract) & 0x01;
            crc >>= 1;
            if (sum) {
                crc ^= 0x8C;
            }
            extract >>= 1;
        }
    }
    return crc;
}

bool
Config::readSlot(uint8_t slotIndex, config_slot_t &slotData)
{
    if (slotIndex * sizeof(config_slot_t) >= EEPROM.length()) {
        return false; 
    }

    EEPROM.get(slotIndex * sizeof(config_slot_t), slotData);
    if (crc8((uint8_t *)&slotData, sizeof(config_slot_t) - 1) == slotData.crc8) {
        return true;
    }
    else {
        return false;
    }
}

bool
Config::writeSlot(uint8_t slotIndex, config_slot_t &slotData)
{
    if (slotIndex * sizeof(config_slot_t) >= EEPROM.length()) {
        return false; 
    }

    slotData.crc8 = crc8((uint8_t *)&slotData, sizeof(config_slot_t) - 1);
    EEPROM.put(slotIndex * sizeof(config_slot_t), slotData);
    return true;
}

bool
Config::freeSlot(uint8_t slotIndex)
{
    if (slotIndex * sizeof(config_slot_t) >= EEPROM.length()) {
        return false; 
    }

    EEPROM.update(slotIndex * sizeof(config_slot_t), 0xff);
    return true;
}

uint8_t
Config::findFirstSlot(uint8_t startId, uint8_t endId)
{
    uint8_t foundSlotIndex = CONFIG_SLOT_NOT_FOUND;
    uint8_t foundSlotId = 0xff;

    if (endId == 0) {
        endId = startId + 1;
    }

    for (uint8_t slotIndex = 0; slotIndex < (EEPROM.length() / sizeof(config_slot_t)); slotIndex++) {

        config_slot_t slotData;
        EEPROM.get(slotIndex * sizeof(config_slot_t), slotData);

        if (slotData.id < startId || slotData.id >= endId || slotData.id >= foundSlotId) {
            continue;
        }
 
        if (crc8((uint8_t *)&slotData, sizeof(config_slot_t) - 1) != slotData.crc8) {
            continue;
        }

        foundSlotIndex = slotIndex;
        foundSlotId = slotData.id;
        if (foundSlotId == startId) {
            break;
        }
    }

    return foundSlotIndex;
}

uint8_t
Config::findFreeSlot(uint8_t startId, uint8_t endId)
{
    uint8_t foundSlotIndex = CONFIG_SLOT_NOT_FOUND;
    uint8_t foundSlotId = 0xff;

    for (uint8_t slotIndex = 0; slotIndex < (EEPROM.length() / sizeof(config_slot_t)); slotIndex++) {

        config_slot_t slotData;
        EEPROM.get(slotIndex * sizeof(config_slot_t), slotData);

        if (slotData.id < startId || slotData.id >= endId || slotData.id >= foundSlotId) {
            continue;
        }
 
        if (crc8((uint8_t *)&slotData, sizeof(config_slot_t) - 1) != slotData.crc8) {
            continue;
        }

        foundSlotIndex = slotIndex;
        foundSlotId = slotData.id;
        if (foundSlotId == startId) {
            break;
        }
    }

    return foundSlotIndex;
}



