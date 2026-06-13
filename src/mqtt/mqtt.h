#pragma once

#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt/mqtt_config.h"
#include "desk_light/desk_light.h"

class Mqtt {
    public:
        explicit Mqtt(const MqttConfig& config);

        void initialize();
        void update();

        void registerLight(DeskLight& light);

    private:
        MqttConfig _config;
        WiFiClient _wifiClient;
        PubSubClient _client;

        static const uint8_t MAX_LIGHTS = 2;
        DeskLight* _lights[MAX_LIGHTS];
        bool _lastState[MAX_LIGHTS];
        uint8_t _lightCount;

        unsigned long _lastReconnectAttempt = 0;
        static const unsigned long _reconnectInterval = 5000;

        bool connect();
        void publishState(uint8_t index, bool on);
};
