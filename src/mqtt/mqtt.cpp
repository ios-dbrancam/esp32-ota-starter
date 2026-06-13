#include "mqtt/mqtt.h"
#include "logger/logger.h"

Mqtt::Mqtt(const MqttConfig& config) : _config(config),
                                       _client(_wifiClient),
                                       _lightCount(0) {}

void Mqtt::initialize() {
    _client.setServer(_config.broker, _config.port);
}

void Mqtt::update() {
    if (!_client.connected()) {
        unsigned long now = millis();
        if (now - _lastReconnectAttempt >= _reconnectInterval) {
            _lastReconnectAttempt = now;
            connect();
        }
        return;
    }

    _client.loop();

    for (uint8_t i = 0; i < _lightCount; i++) {
        bool isOn = _lights[i] -> isOn();
        if (isOn != _lastState[i]) {
            _lastState[i] = isOn;
            publishState(i, isOn);
        }
    }
}

void Mqtt::registerLight(DeskLight& light) {
    if (_lightCount >= MAX_LIGHTS) return;
    _lights[_lightCount] = &light;
    _lastState[_lightCount] = false;
    _lightCount++;
}

bool Mqtt::connect() {
    logger.log("Mqtt connecting...");

    if (_client.connect(_config.clientId, _config.username, _config.password)) {
        logger.log("Mqtt connected!");
        for (uint8_t i = 0; i < _lightCount; i++) {
            _lastState[i] = _lights[i]->isOn();
            publishState(i, _lastState[i]);
        }
        return true;
    }

    logger.log("Mqtt connect failed (" + String(_client.state()) + ")");
    return false;
}

void Mqtt::publishState(uint8_t index, bool isOn) {
    String topic = String(_config.topicRoot) + "/" + String(_lights[index]->getId()) + "/state";
    const char* payload = isOn ? "ON" : "OFF";
    _client.publish(topic.c_str(), payload, true);
    logger.log("Mqtt published: " + String(payload) + " to " + topic);
}
