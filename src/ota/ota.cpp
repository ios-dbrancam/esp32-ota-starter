#include <ArduinoOTA.h>
#include <WiFi.h>
#include "ota/ota.h"
#include "ota/ota_config.h"

Ota::Ota(const OtaConfig& config) : _config(config) {}

void Ota::initialize() {
    ArduinoOTA.setHostname(_config.hostname);
    ArduinoOTA.setPassword(_config.password);

    ArduinoOTA.onStart([]() {
        Serial.println("OTA update starting...");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("OTA update complete, rebooting...");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA progress: %u%%\n", (progress * 100) / total );
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

void Ota::update() {
    ArduinoOTA.handle();
}
