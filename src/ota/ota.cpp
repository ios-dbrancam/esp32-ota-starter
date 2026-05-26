#include <ArduinoOTA.h>
#include <WiFi.h>
#include "ota.h"

void setupOta(const char *hostname, const char *password) {
    ArduinoOTA.setHostname(hostname);
    ArduinoOTA.setPassword(password);

    ArduinoOTA.onStart([]() {
        Serial.println("OTA update starting...");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("OTA update complete, rebooting...");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA progress: %u%%\n", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA error [%u]: ", error);
        switch (error) {
            case OTA_AUTH_ERROR:
                Serial.println("Auth failed");
                break;
            case OTA_BEGIN_ERROR:
                Serial.println("Begin error");
                break;
            case OTA_CONNECT_ERROR:
                Serial.println("Connect error");
                break;
            case OTA_RECEIVE_ERROR:
                Serial.println("Receive failed");
                break;
            case OTA_END_ERROR:
                Serial.println("End failed");
                break;
        }
    });

    ArduinoOTA.begin();
    Serial.println("OTA ready");
}

void handleOta() {
    ArduinoOTA.handle();
}