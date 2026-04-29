#pragma once

#include <vector>
#include <memory>
#include "ringbuffer.h"
#include "logger_registry.h"
#include "logrecord.h"
#include "handler.h"

class Dispatcher {
    public:
        Dispatcher();
    
        void dispatch(std::stop_token stoken);

    private:
        std::vector<std::unique_ptr<Handler>> handlers_;
};