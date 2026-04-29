#pragma once

#include <string>
#include "logrecord.h"

class Logger {
    public: 
        void log(Severity level, 
                 std::string category, 
                 std::string message, 
                 std::string context_key, 
                 std::string context_value);
};