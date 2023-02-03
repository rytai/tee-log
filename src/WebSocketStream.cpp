/*
 * Copyright 2008, 2012-2022 Dirk-Willem van Gulik <dirkx(at)webweaving(dot)org>
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

#include "WebSocketStream.h"

#if (defined(ESP32) || defined(ESP8266))

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

size_t WebSocketStream::write(uint8_t c)
{
    _buff[_at % sizeof(_buff)] = c;
    _at++;
    if (c == '\n' || _at >= sizeof(_buff) - 1)
    {
        _buff[_at++] = 0;
        if (_ws)
        {
            // Serial.println("LINELINE");
            // Serial.printf("--%s--\n", _buff);
            _ws->textAll((const char *)_buff, _at);
        }
        _at = 0;
    }
    return 1;
}

// WebSocketStream::~WebSocketStream()
// {
//     stop();
// }

void WebSocketStream::begin()
{
    Log.printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    if (_ws != NULL)
    {
        return;
    }
    _ws = new AsyncWebSocket(wsPath);

    if (_server == NULL)
    {
        _server = new AsyncWebServer(_webPort);
    }

    _ws->onEvent(onEvent);
    _server->addHandler(_ws);

    Log.printf("Opened websocket on http://%s%s:%d\n", WiFi.localIP().toString().c_str(), wsPath, _webPort);
    MDNS.addService("http", "tcp", _webPort);
};

void WebSocketStream::stop()
{
    // Serial.println("DESTRUCTOR");
    // if (!_server)
    //     return;
    // _server->end();
    // delete _server;
    // _server = NULL;
}

void WebSocketStream::loop()
{
    _ws->cleanupClients(); // TODO: call only once per second or so
}
#endif
