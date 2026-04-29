#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "logrecord.h"

inline std::string colorizeSeverity(Severity level) {
    switch (level) {
        case Severity::Lowest:   return "\x1b[32mLowest\x1b[0m";
        case Severity::Low:      return "\x1b[32mLow\x1b[0m";
        case Severity::Minor:    return "\x1b[36mMinor\x1b[0m";
        case Severity::Major:    return "\x1b[33mMajor\x1b[0m";
        case Severity::Critical: return "\x1b[31mCritical\x1b[0m";
        default:                 return "Unknown";
    }
}

inline void formatPretty(std::ostream& os, LogRecord record) {
    os << "---------------------------------------------------------" << std::endl;
    os << "[" << record.getTime() << "] [" << colorizeSeverity(record.getLevel()) << "] [seq = " << record.getSequenceNumber() << "] [" << record.getThreadID() << "]" << std::endl;
    os << "category: \t" << record.getCategoryName() << std::endl;

    auto& context_map = record.getContext();
    if (context_map.size() == 0) return;

    os << "context : \t" << std::endl;

    // Next block is to "beautify" context output with formatting.
    std::size_t max_key_len = 0;
    for (const auto& [key, value] : context_map) {
        max_key_len = std::max(max_key_len, key.size());
    }

    for (const auto& [key, value] : context_map) {
        os << std::left << std::setw(static_cast<int>(max_key_len)) << key << " : " << value << '\n';
    }
    os << "\n";
    os << "\n";
}


inline void formatCompact(std::ostream& os, LogRecord record) {
    os << "[" << record.getTime() << "] [" << severityToString(record.getLevel()) << "] [seq = " << record.getSequenceNumber() << "] [" << record.getThreadID() << "] [category: \t"
       << record.getCategoryName() << "] ";
    auto& context_map = record.getContext();
    if (context_map.size() == 0) return;
        os << "[context : \t";
    for (const auto& [key, value] : context_map) {
        os << key << " : " << value << " ";
    }
    os << "]" << std::endl;
}