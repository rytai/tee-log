/*
 * Copyright 2023, Tomi Mäkinen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Library that provides a fanout, or T-flow; so that output or logs do
 * not just got to the serial port; but also to a configurable mix of a
 * telnetserver, a webserver, syslog or MQTT.
 */

#ifndef _H_WEBSOCKET_TEE_LOG
#define _H_WEBSOCKET_TEE_LOG
#if (defined(ESP32) || defined(ESP8266))

#include <TLog.h>

#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#endif

#include <ESPAsyncWebServer.h>

class WebSocketStream : public TLog
{
public:
    WebSocketStream(AsyncWebServer *webServer, const char *path = "/ws") : _server(webServer), wsPath(path){};
    WebSocketStream(const uint16_t wsPort = 80, const char *path = "/ws") : _webPort(wsPort), wsPath(path){};
    virtual size_t write(uint8_t c);
    virtual void begin();
    virtual void loop();
    virtual void stop();

private:
    uint16_t _webPort = 0;
    AsyncWebServer *_server = NULL;
    AsyncWebSocket *_ws = NULL;
    uint8_t _buff[1024];
    unsigned long _at = 0;
    const char *wsPath;

protected:
};
#endif
#endif
