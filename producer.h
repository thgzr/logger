#pragma once

#include <chrono>
#include <thread>
#include "logger.h"

class Producer {
    public:
        void generateLog(std::stop_token stoken) {
            Logger l;
            while (!stoken.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                l.log(Severity::Lowest, "INFO", "Testing first log", "AAAA", "BBBB");
                l.log(Severity::Critical, "URGENT", "Testing first log!!!!11", "ZZZZ", "XXXX");
            }
        }
};