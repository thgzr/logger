#pragma once

#include <chrono>
#include <string> 
#include <map>
#include <thread>

enum class Severity {
    Lowest = 0,
    Low,
    Minor,
    Major,
    Critical,
    Unknown
};

inline std::string severityToString(Severity s) {
    switch (s) {
        case Severity::Lowest:      return "Lowest";
        case Severity::Low:         return "Low";     
        case Severity::Minor:       return "Minor";
        case Severity::Major:       return "Major";
        case Severity::Critical:    return "Critical";
    }
    return "Unknown";
}

class LogRecord { 
    public: 
        LogRecord() : 
            time_ (std::chrono::system_clock::now()), 
            level_ (Severity::Unknown), 
            category_name_("not initialized"), 
            message_text_("not initialized"), 
            thread_id_(std::this_thread::get_id()), 
            sequence_number_(-1) {}; 

        LogRecord(
            std::chrono::system_clock::time_point time, 
            Severity level, 
            std::string category_name, 
            std::string message_text,
            std::thread::id thread_id, 
            int sequence_number, 
            std::string context_key, 
            std::string context_value) : 

            time_(time),
            level_(level), 
            category_name_ (category_name),
            message_text_(message_text), 
            thread_id_(thread_id), 
            sequence_number_(sequence_number), 
            context_({{context_key, context_value}}) {}; 
        
        
        void add_context(std::string context_key, std::string context_value) { 
            context_.insert({context_key, context_value}); 
        }
        
        std::chrono::system_clock::time_point getTime() { return time_; }
        Severity getLevel() { return level_; }
        std::string getCategoryName() { return category_name_; }
        std::string getMessageText() { return message_text_; }
        std::thread::id getThreadID() { return thread_id_; }
        int getSequenceNumber() { return sequence_number_; }
        std::map<std::string, std::string>& getContext() { return context_; }

        
    private: 
        std::chrono::system_clock::time_point time_; 
        Severity level_; 
        std::string category_name_; 
        std::string message_text_; 
        std::thread::id thread_id_; 
        int sequence_number_; 
        std::map<std::string, std::string> context_;   
};