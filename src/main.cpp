#include <Arduino.h>
#include <ArduinoJson.h>
#include <ModuleCore.h>
#include "wifi_secrets.h"

#define DEAD_ZONE 5
#define SET_POINT 512

#define DEAD_MIN SET_POINT-DEAD_ZONE
#define DEAD_MAX SET_POINT+DEAD_ZONE

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

void setMotor(uint8_t pinEN, uint8_t pinPWM, uint16_t pwm) {
    if(pwm > DEAD_MIN && pwm < DEAD_MAX) {
        digitalWrite(pinEN, LOW);
        analogWrite(pinPWM, SET_POINT);
    } else {
        digitalWrite(pinEN, HIGH);
        analogWrite(pinPWM, pwm);
    }

}

void setMotorSpeedsListener(DynamicJsonDocument inDoc){
    Serial.println("SetMotorSpeeds");
    setMotor(EN12, PWM12, inDoc["m1"].as<uint16_t>());
    setMotor(EN34, PWM34, inDoc["m2"].as<uint16_t>());
}

void setM1SpeedListener(DynamicJsonDocument inDoc){
    //Serial.println("SetM1Speed");
    //Serial.println(inDoc["speed"].as<uint16_t>());
    //inDoc["speed"].as<uint16_t>()
    setMotor(EN12, PWM12, inDoc["speed"].as<uint16_t>());


}

void setM2SpeedListener(DynamicJsonDocument inDoc){
    //Serial.println("SetM2Speed");
    setMotor(EN34, PWM34, inDoc["speed"].as<uint16_t>());
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
    core.forceWiFiPassword(WIFI_PASSWORD);
    core.forceWiFiSSID(WIFI_SSID);

    core.begin();
}

void loop() {
    core.loop();
}

