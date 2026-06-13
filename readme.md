# ESP32 OTA Starter
A starter template for ESP32 projects with WiFi, OTA updates, and mDNS preconfigured. Clone it, fill in your credentials, and start building — your firmware will be updateable over the air from the first commit.
## What's included
- **WiFi connection management** with static IP support and automatic reconnection
- **OTA (over-the-air) firmware updates** via ArduinoOTA
- **mDNS** so the device is reachable by hostname (e.g. `myproject.local`)
- **Modular project structure** ready to extend
- **Secrets handling** that keeps credentials out of git
## Project structure
```
src/
├── config/
│   ├── config.h               # Pins, network, hostname
│   ├── secrets.h.template     # Credential template (rename and fill in)
│   └── secrets.h              # Real credentials (git-ignored)
├── network/
│   ├── network_config.h
│   ├── network.h
│   └── network.cpp            # WiFi, static IP, mDNS, reconnect
├── ota/
│   ├── ota_config.h
│   ├── ota.h
│   └── ota.cpp                # OTA update handler
└── main.cpp
```
## First-time setup
### 1. Rename the template files
Any file with the `.template` suffix is a placeholder — rename it (dropping `.template`) and fill in your real values:
### 2. Configure your project
Edit `src/config/config.h`:
- **Static IP** — adjust `localIp`, `gateway`, `subnet`, `dns` for your network (or leave defaults if your subnet is `192.168.0.x`)
- **Hostname** — what the device will be reachable as on the network (e.g. `myproject` → `myproject.local`)
### 3. First flash — over USB
The very first flash **must be done via USB**, because OTA can't update a device that doesn't have OTA running on it yet.
Open `platformio.ini` and make sure the OTA upload lines are **commented out**:
Plug the board in over USB and upload:
### 4. Subsequent flashes — Over The Air
Once OTA is running on the device, **uncomment** the three lines you commented out before:
Make sure `upload_port` matches your hostname and `--auth` matches your `otaPassword`. ## Using this as a base for new projects
## Requirements
- [PlatformIO](https://platformio.org/)
- An ESP32 board — default target is the Seeed XIAO ESP32-C3, but any ESP32 variant works by changing `board` in `platformio.ini`
- A 2.4 GHz WiFi network the device can join
## Example
This repo includes an example: a two-channel desk light controller with physical buttons, gamma-corrected dimming, long-press shortcuts, and smooth eased fades. It adds three new modules (`button/`, `desk_light/`, and `config/pwm_config.h`) on top of this base while leaving `network/` and `ota/` untouched, so it doubles as a worked example of how to extend the starter without disturbing what's already there.
