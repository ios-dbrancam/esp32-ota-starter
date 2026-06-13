#pragma once

#include "ota/ota_config.h"

class Ota {
    public:
        explicit Ota(const OtaConfig& config);
        void initialize();
        void update();
    
    private:
        OtaConfig _config;
};