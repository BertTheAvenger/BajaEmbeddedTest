#include <Arduino.h>
#include <ArduinoJson.h>
#include <ModuleCore.h>

#define EN12 D1
#define EN34 D7
#define PWM12 D3
#define PWM34 D4

ModuleCore core;

void statusRequestListener(DynamicJsonDocument inDoc) {
    Serial.println("LISTENER FOR 'statusRequest'");

    //Create and setup the JSON document to send back.
    StaticJsonDocument<512> doc;

    doc["temp"] = 25.332f;
    doc["m1Pos"] = 500;
    doc["m2Pos"] = 420;

    core.sendDoc("status", doc);
}

void setMotorSpeedsListener(DynamicJsonDocument inDoc){
    Serial.println("SetMotorSpeeds");
}

void setM1SpeedListener(DynamicJsonDocument inDoc){
    Serial.println("SetM1Speed");
    Serial.println(inDoc["speed"].as<uint16_t>());
    analogWrite(PWM12, inDoc["speed"].as<uint16_t>());
    digitalWrite(EN12, HIGH);
}

void setM2SpeedListener(DynamicJsonDocument inDoc){
    Serial.println("SetM2Speed");
    analogWrite(PWM34, inDoc["speed"].as<uint16_t>());
    digitalWrite(EN34, HIGH);
}

void setup() {
    pinMode(EN12, OUTPUT);
    pinMode(EN34, OUTPUT);

    pinMode(PWM12, OUTPUT);
    pinMode(PWM34, OUTPUT);

    digitalWrite(EN12, LOW);
    digitalWrite(EN34, LOW);

    Serial.begin(115200);
    Serial.println("Starting...");
    core.setType("8266_DUAL_ROOMBA_MOTOR_DRIVER");

    core.addTopicListener("statusRequest", statusRequestListener);
    core.addTopicListener("setMotorSpeeds", setMotorSpeedsListener);
    core.addTopicListener("setM1Speed", setM1SpeedListener);
    core.addTopicListener("setM2Speed", setM2SpeedListener);

    core.forceServerIP("192.168.1.111");
    core.forceServerPath("/");
    core.forceServerPort(420);
    core.forceWiFiPassword("szastarka1!");
    core.forceWiFiSSID("KA45A");

    core.begin();
}

void loop() {
    core.loop();
}

