#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include "logrecord.h"
#include "logger.h"
#include "formatter.h"

class Handler {
    public:
        virtual ~Handler() = default;
        virtual bool handle(LogRecord& record) = 0;
};

class LevelFilter : public Handler {
    public:
        LevelFilter(Severity level) : min_level_(level) {}
        bool handle(LogRecord& record) override {
            return record.getLevel() >= min_level_;
        }
    
    private:
        Severity min_level_;
};

class ContextEnricher : public Handler {
    public:
        ContextEnricher(std::string key, std::string value) : key_(key), value_(value) {}
        bool handle(LogRecord& record) override {
            record.add_context(key_, value_);
            record.add_context("password", "12345");
            return true;
        }
    
    private:
        std::string key_;
        std::string value_;
};

class Redactor : public Handler {
    public:
        bool handle(LogRecord& record) override {
            auto& context_map = record.getContext();
            for (auto& [key, value] : context_map) {
                if (key == "token" || key == "password") value = "[REDACTED]";
            }
            return true;
        }
};

class ConsoleSink : public Handler {
    bool handle(LogRecord& record) override {
        formatPretty(std::cout, record);
        return true;
    }
};

class FileSink : public Handler {
    public:
        FileSink() : 
            max_bytes_(1000000),
            current_index_(0)
            {}; 

        bool handle(LogRecord& record) override {
            if(!out_.is_open()) 
                openNewFile();
            
            formatCompact(out_, record);
            rotateIfNeeded();

            return true;
        }

    private:
        void openNewFile() {
            auto new_file_name = "logs_" + std::to_string(current_index_) + ".txt";
            ++current_index_;
            out_.close();
            out_.open(new_file_name);
        }

        void rotateIfNeeded() {
            auto size = out_.tellp();
            if (size == -1) return;
            if (size > max_bytes_) {
                openNewFile();
            }
        }
        
        std::ofstream out_;
        std::size_t max_bytes_;
        std::size_t current_index_;
};