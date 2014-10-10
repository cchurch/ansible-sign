/*
 * Pixel API Demo with Yún Bridge (based on Arduino Yún Bridge example)
 * Copyright 2014 Chris Church <chris@ninemoreminutes.com>
 * 
 * Example REST URLs (return JSON):
 * - /arduino/pixel/0/96/64/32 -> set pixel 0 to rgb(96,64,32)
 * - /arduino/pixel/-/32/64/96 -> set all pixels to rgb(32,64,96)
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

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Adafruit_NeoPixel.h>

#define NP_COUNT 60
#define NP_PIN 12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NP_COUNT, NP_PIN, NEO_GRB + NEO_KHZ800);

// Listen on default port 5555, the webserver on the Yún
// will forward there all the HTTP requests for us.
YunServer server;

void setup() {
  pixels.begin();
  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // Get clients coming from server
  YunClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }

  delay(10); // Poll every 10ms
}

void process(YunClient client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "digital" command?
  if (command == "pixel") {
    pixelCommand(client);
  }
}

void pixelCommand(YunClient client) {
  int c1 = -1, c2 = -1, p0 = -1, p1 = -1, p = 0, r = -1, g = -1, b = -1;
  
  c1 = client.peek();
  if (c1 < 0) {
    p0 = 0;
    p1 = NP_COUNT - 1;
  }
  else if (c1 == '-') {
    c1 = client.read();
    p0 = 0;
  }
  else if (c1 >= '0' && c1 <= '9') {
    p0 = client.parseInt();
  }
  else {
    client.print(F("error"));
    return;
  }

  if (c1 >= 0) {
    c2 = client.peek();
    if (c1 == '-' && c2 < 0) {
      p1 = NP_COUNT - 1;
    }
    else if (c2 < 0) {
      p1 = p0;
    }
    else if (c2 == '/') {
      if (c1 == '-') {
        p1 = NP_COUNT - 1;
      }
      else {
        p1 = p0;
      }
      c2 = client.read();
    }
    else if (c2 >= '0' && c2 <= '9') {
      p1 = client.parseInt();
    }
    else {
      client.print(F("error"));
      return;
    }
  }

  if (c2 == '/' && client.peek() > 0) {
    r = client.parseInt();
    g = client.parseInt();
    b = client.parseInt();
  }
  
  client.print(F("[\n"));
  for (p = p0; p <= p1; p++) {
    if (r >= 0) {
      pixels.setPixelColor(p, pixels.Color(r, g, b));
    }
    client.print(F("    {\n        \"p\": "));
    client.print(p);
    client.print(F(",\n        \"r\": "));
    client.print(r);
    client.print(F(",\n        \"g\": "));
    client.print(g);
    client.print(F(",\n        \"b\": "));
    client.print(b);
    if (p < p1) {
      client.print(F("\n    },\n"));
    }
    else {
      client.print(F("\n    }\n"));
    }
  }
  client.print(F("]\n"));
  
  if (r >= 0) {
    pixels.show();
  }

}
