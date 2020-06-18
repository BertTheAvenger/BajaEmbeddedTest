//
// Created by Dominic on 6/13/2020.
//
#define LOG Serial.println
#include "ModuleCore.h"
#include "strings.h"

ModuleCore::ModuleCore() {

}

ModuleCore::~ModuleCore() {

}

void ModuleCore::wsEvent(WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            wsDisconnected(payload, length);
            LOG("[WSc] Disconnected!");
            break;
        case WStype_CONNECTED:
            wsConnected(payload, length);
            break;
        case WStype_TEXT:
            wsTxtMessage(payload, length);
            break;
        case WStype_BIN:
            wsBinMessage(payload, length);
            LOG("payload");
            break;
        case WStype_PING:
        case WStype_PONG:
        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            break;
    }
}




void ModuleCore::wsTxtMessage(uint8_t *payload, size_t length) {
    char* inputBuf = new char[length];
    memcpy(inputBuf, payload, length);


    DynamicJsonDocument* doc = new DynamicJsonDocument(2048);
    deserializeJson((*doc), inputBuf);

    if((*doc)["topic"].is<char*>()) {

        const char* topicStr = (*doc)["topic"].as<const char*>();
        //Serial.print("Got topic: ");
        //Serial.println(topicStr);

        auto listener = topicListeners.getValueOf(topicStr);
        //Serial.println("Got val of listener");

        if(listener) {
            //Serial.println("Got listener");
            queuedEvent *ev = new queuedEvent{listener, doc, inputBuf};
            //Serial.println("Created struct");
            eventQueue.enqueue(ev);
            //Serial.println("Equeueed struct");
        } else {
            Serial.println("Didn't find topic listener");
        }
    }
}




void ModuleCore::wsBinMessage(uint8_t *payload, size_t length) {

}




void ModuleCore::setType(String type) {
    this->type = type;
}




void ModuleCore::wsConnected(uint8_t *payload, size_t length) {

    Serial.println("Connected!");

    StaticJsonDocument<256> doc; // fixed size

    doc["muid"] = WiFi.macAddress();
    doc["type"] = type;

    sendDoc("identification", doc);
}



void ModuleCore::wsDisconnected(uint8_t *payload, size_t length) {

}




void ModuleCore::addTopicListener(const char* topic, void (*listener)(DynamicJsonDocument)) {
    if(topicIterator < MAP_SIZE) {
        topicListeners.set(topic, listener);
        topicIterator++;
    }
}





void ModuleCore::loop() {
    //Handle a queued event.
    if(eventQueue.count() > 0) {
        queuedEvent* e = eventQueue.dequeue();
        e->listener( *(e->doc) );

        delete e->doc;
        delete [] e->input;
        delete e;
    }


    webSocket.loop();
}

void ModuleCore::begin() {

    WiFi.begin(WiFiSSID, WiFiPasswd);
    Serial.print("Connecting");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    webSocket.begin(ServerIP, ServerPort, ServerPath);

    //AAAHHHHHHHH I HATE CPP! WHY CAN I NOT JUST PASS A MEMBER FUNCTION?
    //Create a lambda to pass websocket events to this object's handlers.
    auto socketEvt = [this](WStype_t type, uint8_t *payload, size_t length){ this->wsEvent(type, payload, length); };
    webSocket.onEvent(socketEvt);

    //Enable verifications, for now.
    webSocket.setReconnectInterval(5000);
    webSocket.enableHeartbeat(15000, 3000, 2);
    Serial.println("Finished initialization.");
}


void ModuleCore::sendDoc(const char *topic, JsonDocument &doc) {
    doc["topic"] = topic; //Add the topic.

    char buf[2048];
    serializeJson(doc, buf);
    webSocket.sendTXT(buf);
}


void ModuleCore::forceServerIP(const char *v) {
    ServerIP = v;
}

void ModuleCore::forceServerPort(uint16_t v) {
    ServerPort = v;
}

void ModuleCore::forceServerPath(const char *v) {
    ServerPath = v;
}

void ModuleCore::forceWiFiSSID(const char *v) {
    WiFiSSID = v;
}

void ModuleCore::forceWiFiPassword(const char *v) {
    WiFiPasswd = v;
}
