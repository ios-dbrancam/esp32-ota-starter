#include <WiFi.h>
#include "logger/logger.h"
#include "logger/logger_config.h"

static WiFiServer telnetServer(0);
static WiFiClient telnetClient;

static const LoggerConfig config = {
    .port = 23,
    .serialMirror = true
};

Logger logger(config);

Logger::Logger(const LoggerConfig& config) : _config(config) {}

void Logger::initialize() {
    telnetServer = WiFiServer(_config.port);
    telnetServer.begin();
    telnetServer.setNoDelay(true);
}

void Logger::update() {
    if (telnetServer.hasClient()) {
        if (telnetClient.connected()) {
            telnetClient.stop();
        }
        telnetClient = telnetServer.accept();
    }
}

void Logger::log(const String& message) {
    if (_config.serialMirror) {
        Serial.println(message);
    }
    if (telnetClient && telnetClient.connected()) {
        telnetClient.println(message);
    }
}
