#pragma once

#include <Arduino.h>
#include "logger/logger_config.h"

class Logger {
    public:
        explicit Logger(const LoggerConfig& config);

        void initialize();
        void update();
        
        void log(const String& message);
    
    private:
        LoggerConfig _config;
};

extern Logger logger;
