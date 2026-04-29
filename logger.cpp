#include "logrecord.h"
#include "logger_registry.h"
#include "logger.h"
#include <chrono>
#include <string>
#include <atomic>
#include <thread>

namespace {
    static std::atomic<uint64_t> sequence_number{0};
}

void Logger::log(Severity level, std::string category, std::string message, std::string context_key, std::string context_value) {
    auto now_utc = std::chrono::system_clock::now();
    auto thread_id = std::this_thread::get_id();
    uint64_t seq = sequence_number.fetch_add(1, std::memory_order_relaxed);
    LogRecord record(now_utc, level, category, message, thread_id, seq, context_key,  context_value);

    
    LoggerRegistry::getInstance().getBuffer().push(record);
}

