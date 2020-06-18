//
// Created by Dominic on 6/13/2020.
//
#include <Arduino.h>
#include <WebSockets.h>
#include "HashMap2.h"
#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <QueueArray.h>

#ifndef MODULECORE_H
#define MODULECORE_H

#define MAP_SIZE 50

struct moduleData {
    String serverIP = "";
    String apSSID = "";
    String apPasswd = "";
};

struct queuedEvent {
    void (*listener)(DynamicJsonDocument);
    DynamicJsonDocument* doc;
    char* input;
};

class ModuleCore {
private:
    WebSocketsClient webSocket;

    //WS Event handler functions
    void wsEvent(WStype_t type, uint8_t *payload, size_t length);
    void wsDisconnected(uint8_t *payload, size_t length);
    void wsConnected(uint8_t *payload, size_t length);
    void wsTxtMessage(uint8_t *payload, size_t length);
    void wsBinMessage(uint8_t *payload, size_t length);

    //Vars for handling listener calling.
    uint16_t topicIterator = 0;
    HashMap2<void (*)(DynamicJsonDocument), 100> topicListeners;
    QueueArray<queuedEvent*> eventQueue;

    //Behavior vars
    String type= "NO_TYPE";
    const char* ServerIP = "";
    const char* ServerPath = "/";
    uint16_t ServerPort = 80;

    const char* WiFiSSID = "";
    const char* WiFiPasswd = "";


public:

    ModuleCore();
    ~ModuleCore();
    void setType(String type);
    void addTopicListener(const char*, void (*)(DynamicJsonDocument)); //Events are queued and called in the normal loop.
    void begin();
    void loop();
    void sendDoc(const char*, JsonDocument& );

    //Connection overrides (For testing purposes)
    void forceServerIP(const char*);
    void forceServerPort(uint16_t);
    void forceServerPath(const char*);
    void forceWiFiSSID(const char*);
    void forceWiFiPassword(const char*);
};


#endif
