/*
 * Ansible Sign Blinky Lights - Remote Config via Bridge
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

#include "Remote.h"

Remote::Remote()
{
}

Remote::~Remote()
{
}

void
Remote::setup()
{
    Bridge.begin();
    m_bridgeServer.listenOnLocalhost();
    m_bridgeServer.begin();
}

void
Remote::loop()
{
    BridgeClient client = m_bridgeServer.accept();

    if (client) {
        handleClient(client);
    }
}

void
Remote::handleClient(const BridgeClient &client)
{
    String command = client.readStringUntil('/');

    if (command == "colors") {
        colorsCommand(client);
    }
    else {
        client.println(F("Not Found"));
    }
    client.stop();
}

void
Remote::colorsCommand(const BridgeClient &client)
{
    client.println(client.readString());    
}

