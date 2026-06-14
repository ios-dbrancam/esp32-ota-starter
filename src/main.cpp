#include <Arduino.h>
#include "config/config.h"
#include "config/pwm_config.h"
#include "config/secrets.h"
#include "desk_light/desk_light.h"
#include "logger/logger.h"
#include "mqtt/mqtt.h"
#include "mqtt/mqtt_config.h"
#include "network/network.h"
#include "network/network_config.h"
#include "ota/ota.h"

const PwmConfig pwmConfig = {
  .frequency = pwmFrequency,
  .resolution = pwmResolution,
  .lowerLimit = pwmMin,
  .upperLimit = pwmMax,
  .steps = pwmSteps,
  .gamma = ledGamma
};

DeskLight leftLight(
  leftPwmPin,
  "left",
  leftBrightnessUp,
  leftBrightnessDown,
  pwmConfig,
  debounceTime,
  longPressTime
);

DeskLight rightLight(
  rightPwmPin,
  "right",
  rightBrightnessUp,
  rightBrightnessDown,
  pwmConfig,
  debounceTime,
  longPressTime
);

const NetworkConfig networkConfig = {
  .ssid = networkSsid,
  .password = networkPwd,
  .localIp = localIp,
  .gateway = gateway,
  .subnet = subnet,
  .dns = dns,
  .hostname = hostname
};

const OtaConfig otaConfig = {
  .hostname = hostname,
  .password = otaPassword
};

NetworkController network(networkConfig);
Ota ota(otaConfig);

const MqttConfig mqttConfig = {
  .broker = mqttBroker,
  .port = mqttPort,
  .clientId = mqttClientId,
  .username = mqttUsername,
  .password = mqttPassword,
  .topicRoot = mqttRoot,
  .deviceName = deviceName
};

Mqtt mqtt(mqttConfig);

void setup() {
  Serial.begin(115200);
  network.initialize();
  logger.initialize();
  ota.initialize();

  leftLight.initialize();
  rightLight.initialize();

  mqtt.initialize();
  mqtt.registerLight(leftLight);
  mqtt.registerLight(rightLight);
}

void loop() {
  network.update();
  logger.update();
  mqtt.update();
  ota.update();
  leftLight.update();
  rightLight.update();
}
