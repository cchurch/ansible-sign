/*
 * Ansible Sign Blinky Lights - Sign
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

#include "Sign.h"

Sign::Sign(int pixelCount, int pixelPin, int motionPin, int soundInput) :
    m_motionPin(-1), m_soundInput(-1), m_lastSound(0),
    m_pixels(Adafruit_NeoPixel(0, -1, NEO_GRB + NEO_KHZ800))
{
    setPixelCount(pixelCount);
    setPixelPin(pixelPin);
    setMotionPin(motionPin);
    setSoundInput(soundInput);
    m_pixels.setBrightness(127);
}

Sign::Sign(config_pins_t &pins_t, config_options_t &options_t) :
    m_motionPin(-1), m_soundInput(-1), m_lastSound(0),
    m_pixels(Adafruit_NeoPixel(0, -1, NEO_GRB + NEO_KHZ800))
{
    if (pins_t.motionInput != 0xff) {
        setMotionPin(pins_t.motionInput);
    }
    if (pins_t.soundInput != 0xff) {
        setSoundInput(pins_t.soundInput);
    }
}


Sign::~Sign()
{  
}

void
Sign::setup()
{
    m_pixels.begin();
    m_pixels.clear();
    m_pixels.show();
}

void
Sign::setMotionPin(int pin)
{
    if (m_motionPin != pin) {
        if (m_motionPin >= 0) {
            pinMode(m_motionPin, INPUT);
            digitalWrite(m_motionPin, LOW);
        }
        m_motionPin = pin;
        if (m_motionPin >= 0) {
            pinMode(m_motionPin, INPUT);
            digitalWrite(m_motionPin, LOW);
        }
    }
}

void
Sign::setPixelCount(int count)
{
    m_pixels.updateLength((uint16_t)count);
}

void
Sign::setPixelPin(int pin)
{
    m_pixels.setPin(pin);
}

void
Sign::setSoundInput(int input)
{
    m_soundInput = input;
}

void
Sign::setBrightness(uint8_t brightness)
{
    m_pixels.setBrightness(brightness);
}

void
Sign::setPixelColor(uint16_t pixel, const Color &color)
{
    Color actualColor = color;
    actualColor.applyMotion(getMotion());
    actualColor.applySound(getSound());
    m_pixels.setPixelColor(pixel, actualColor.m_red, actualColor.m_green, actualColor.m_blue); 
}

void
Sign::showPixels()
{
    m_pixels.show();  
}

void
Sign::wait(int ms)
{
    if (m_soundInput >= 0) {
        unsigned long start_ms = millis();
        unsigned int d_min = 1024;
        unsigned int d_max = 0;
        unsigned int d_sample;

        while (millis() - start_ms < ms) {
            d_sample = analogRead(m_soundInput);
            if (d_sample < 1024) {
                if (d_sample > d_max) {
                    d_max = d_sample;
                }
                if (d_sample < d_min) {
                    d_min = d_sample;
                }
            }
        }
        if (d_max > 511) {
            d_max = 511;
        }
        m_lastSound = (d_max - d_min) / 2;
    }
    else {
        delay(ms);
    }
}

int
Sign::getMotionPin() const
{
    return m_motionPin;
}

int
Sign::getPixelCount() const
{
    return m_pixels.numPixels();
}

int
Sign::getPixelPin() const
{
    return m_pixels.getPin();
}

int
Sign::getSoundInput() const
{
    return m_soundInput;
}

uint8_t
Sign::getBrightness() const
{
    return m_pixels.getBrightness();
}

uint8_t
Sign::getMotion() const
{
    return (m_motionPin >= 0) ? digitalRead(m_motionPin) : 0;
}

uint8_t
Sign::getSound() const
{
    return m_lastSound;
}

