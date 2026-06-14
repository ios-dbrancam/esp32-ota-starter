#include "mqtt/mqtt.h"
#include "logger/logger.h"

Mqtt::Mqtt(const MqttConfig& config) : _config(config),
                                       _client(_wifiClient),
                                       _lightCount(0) {}

void Mqtt::initialize() {
    _instance = this;
    _client.setServer(_config.broker, _config.port);
    _client.setCallback(onMessage);
    _client.setBufferSize(512);
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

Mqtt* Mqtt::_instance = nullptr;

void Mqtt::onMessage(char* topic, byte* payload, unsigned int length) {
    if (_instance != nullptr) {
        _instance->handleMessage(topic, payload, length);
    }
}

void Mqtt::handleMessage(char* topic, byte* payload, unsigned int length) {
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';

    logger.log("Mqtt received: " + String(message) + " on " + String(topic));

    for (uint8_t i = 0; i < _lightCount; i++) {
        String commandTopic = String(_config.topicRoot) + "/" + String(_lights[i]->getId()) + "/set";

        if (commandTopic == String(topic)) {
            if (strcmp(message, "ON") == 0) {
                _lights[i]->turnOn();
            } else if (strcmp(message, "OFF") == 0) {
                _lights[i]->turnOff();
            }
            return;
        }
    }
}

bool Mqtt::connect() {
    logger.log("Mqtt connecting...");

    if (_client.connect(_config.clientId, _config.username, _config.password)) {
        logger.log("Mqtt connected!");

        for (uint8_t i = 0; i < _lightCount; i++) {
            _lastState[i] = _lights[i]->isOn();
            publishState(i, _lastState[i]);
            publishDiscovery(i);
        }

        for (uint8_t i = 0; i < _lightCount; i++) {
            String topic = String(_config.topicRoot) + "/" + String(_lights[i]->getId()) + "/set";
            _client.subscribe(topic.c_str());
            logger.log("Mqtt subscribed to: " + topic);
        }

        return true;
    }

    logger.log("Mqtt connect failed (" + String(_client.state()) + ")");
    return false;
}

void Mqtt::publishDiscovery(uint8_t index) {
    String id = String(_lights[index]->getId());
    String uniqueId = String(_config.clientId) + "-" + id;
    String stateTopic = String(_config.topicRoot) + "/" + id + "/state";
    String commandTopic = String(_config.topicRoot) + "/" + id + "/set";
    String discoveryTopic = "homeassistant/light/" + uniqueId + "/config";

    String payload =
        String("{\"name\":\"") + id + "\","
        "\"unique_id\":\"" + uniqueId + "\","
        "\"state_topic\":\"" + stateTopic + "\","
        "\"command_topic\":\"" + commandTopic + "\","
        "\"payload_on\":\"ON\","
        "\"payload_off\":\"OFF\","
        "\"retain\":true,"
        "\"device\":{"
            "\"identifiers\":[\"" + String(_config.clientId) + "\"],"
            "\"name\":\"" + String(_config.deviceName) + "\","
            "\"model\":\"XIAO ESP32 C3\","
            "\"manufacturer\":\"Seeed Studio\""
        "}}";
    
    _client.publish(discoveryTopic.c_str(), payload.c_str(), true);
    logger.log("Mqtt discovery published for: " + id);
}

void Mqtt::publishState(uint8_t index, bool isOn) {
    String topic = String(_config.topicRoot) + "/" + String(_lights[index]->getId()) + "/state";
    const char* payload = isOn ? "ON" : "OFF";
    _client.publish(topic.c_str(), payload, true);
    logger.log("Mqtt published: " + String(payload) + " to " + topic);
}
