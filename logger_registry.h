#pragma once

#include "ringbuffer.h"

class LoggerRegistry {
    public:
        static LoggerRegistry& getInstance() {
            static LoggerRegistry instance_;
            return instance_;
        }

        RingBuffer& getBuffer() { return buffer_; }

    private:
        RingBuffer buffer_;
        LoggerRegistry() = default;
};
