#include <Arduino.h>
#include "config/config.h"
#include "config/pwm_config.h"
#include "config/secrets.h"
#include "desk_light/desk_light.h"
#include "network/network.h"
#include "network/network_config.h"
#include "ota/ota.h"

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

const PwmConfig pwmConfig = {
  .frequency = pwmFrequency,
  .resolution = pwmResolution,
  .lowerLimit = pwmMin,
  .upperLimit = pwmMax,
  .steps = pwmSteps,
  .gamma = ledGamma
};

Network network(networkConfig);
Ota ota(otaConfig);

DeskLight leftLight(leftPwmPin, leftBrightnessUp, leftBrightnessDown, pwmConfig, debounceTime, longPressTime);
DeskLight rightLight(rightPwmPin, rightBrightnessUp, rightBrightnessDown, pwmConfig, debounceTime, longPressTime);

void setup() {
  Serial.begin(115200);
  network.initialize();
  ota.initialize();
  leftLight.initialize();
  rightLight.initialize();
}

void loop() {
  network.update();
  ota.update();
  leftLight.update();
  rightLight.update();
}
