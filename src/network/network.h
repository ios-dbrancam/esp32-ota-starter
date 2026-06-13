#pragma once

#include "network/network_config.h"

class NetworkController {
    public:
        explicit NetworkController(const NetworkConfig& config);
        void initialize();
        void update();
    
    private:
        NetworkConfig _config;
        unsigned long _lastAttempt = 0;
        static const unsigned long _retryInterval = 30 * 1000;
        bool _dnsConfigured = false;

        void configureStaticIp();
        void configureDns();
};
