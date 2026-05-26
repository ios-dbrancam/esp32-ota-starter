#include <WiFi.h>
#include <ESPmDNS.h>
#include "network/network.h"

static void configStaticIp(
        IPAddress localIp,
        IPAddress gateway,
        IPAddress subnet,
        IPAddress dns
    ) {
    WiFi.config(localIp, gateway, subnet, dns);
}

static void configDns(const char* hostname) {
    if (MDNS.begin(hostname)) {
        Serial.println("mDNS started - http://" + String(hostname) + ".local");
    } else {
        Serial.println("mDNS failed to start");
    }
}

void setupNetwork(
        const char* ssid,
        const char* password,
        IPAddress localIp,
        IPAddress gateway,
        IPAddress subnet,
        IPAddress dns,
        const char* hostname
    ) {

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);

    if ((localIp != IPAddress()) && (gateway != IPAddress()) && (subnet != IPAddress()) && (dns != IPAddress())) {
        configStaticIp(localIp, gateway, subnet, dns);
    }

    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected\nIP: " + WiFi.localIP().toString());
    Serial.println("MAC: " + WiFi.macAddress());

    if (hostname != nullptr) {
        configDns(hostname);
    }
}

void ensureNetwork() {
    static unsigned long lastAttempt = 0;
    const unsigned long retryInterval = 30000;

    if (WiFi.status() == WL_CONNECTED) return;

    unsigned long now = millis();
    if (now - lastAttempt >= retryInterval) {
        lastAttempt = now;
        Serial.println("WiFi lost, reconnecting...");
        WiFi.reconnect();
    }
}