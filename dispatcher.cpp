#include "dispatcher.h"
#include "logrecord.h"
#include "logger_registry.h"
#include "handler.h"
#include "logger.h"
#include <vector>


Dispatcher::Dispatcher() {
    handlers_.push_back(std::make_unique<LevelFilter>(Severity::Lowest));
    handlers_.push_back(std::make_unique<ContextEnricher>("ctx_enricher1", "ctx_enricher2"));
    handlers_.push_back(std::make_unique<Redactor>());
    handlers_.push_back(std::make_unique<ConsoleSink>());
    handlers_.push_back(std::make_unique<FileSink>());
}

void Dispatcher::dispatch(std::stop_token stoken) {
    LogRecord record;
    auto& buffer = LoggerRegistry::getInstance().getBuffer();

    while (!stoken.stop_requested()) {
        if (!buffer.pop(record)) break;
        for (auto& handler : handlers_) {
            if (!handler->handle(record)) break;
        }
    }
}