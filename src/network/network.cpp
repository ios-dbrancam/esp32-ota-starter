#include <WiFi.h>
#include <ESPmDNS.h>
#include "network/network.h"
#include "network/network_config.h"
#include "logger/logger.h"

Network::Network(const NetworkConfig& config) : _config(config) {}

void Network::initialize() {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);

    if ((_config.localIp != IPAddress()) && (_config.gateway != IPAddress()) && (_config.subnet != IPAddress()) && (_config.dns != IPAddress())) {
        configureStaticIp();
    }

    WiFi.begin(_config.ssid, _config.password);
    Serial.println("Connecting to WiFi");

    unsigned long startAttemp = millis();
    while(WiFi.status() != WL_CONNECTED) {
        if (millis() - startAttemp > _config.connectTimeout) {
            Serial.println("WiFi connection timed out, continuing without network");
            return;
        }
        delay(500);
        Serial.print(".");
    }
    logger.log("\nConnected\nIP: " + WiFi.localIP().toString());
    logger.log("MAC: " + WiFi.macAddress());

    if (_config.hostname != nullptr) {
        configureDns();
    }
}

void Network::update() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!_dnsConfigured && _config.hostname != nullptr) {
            configureDns();
        }
        return;
    }

    unsigned long now = millis();
    if (now - _lastAttempt >= _retryInterval) {
        _lastAttempt = now;
        Serial.println("WiFi lost, reconnecting...");
        WiFi.reconnect();
        _dnsConfigured = false;
    }
}

void Network::configureStaticIp() {
    WiFi.config(_config.localIp, _config.gateway, _config.subnet, _config.dns);
}

void Network::configureDns() {
    if (MDNS.begin(_config.hostname)) {
        logger.log("mDNS started - http://" + String(_config.hostname) + ".local");
        _dnsConfigured = true;
    } else {
        logger.log("mDNS failed to start");
    }
}
