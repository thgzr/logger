#pragma once

#include "logrecord.h"
#include <array>
#include <mutex>
#include <condition_variable>

class RingBuffer {
    public:
        RingBuffer() : push_ptr_(0), pop_ptr_(0), current_capacity_(0), drop_counter_(0), shutting_down_(false) {};

        void push(const LogRecord& rec) {
            std::unique_lock<std::mutex> lock(mtx_);
            buffer_[push_ptr_] = rec;                                              
            push_ptr_ = (push_ptr_ + 1) % BUFFER_SIZE;           

            if (current_capacity_ < BUFFER_SIZE) ++current_capacity_;
            else {
                pop_ptr_ = (pop_ptr_ + 1) % BUFFER_SIZE;
                ++drop_counter_;
            }
            cv_.notify_one();
        }

        bool pop(LogRecord& out) {
            std::unique_lock<std::mutex> lock(mtx_);

            if (shutting_down_) return false;

            if (current_capacity_ == 0) {
                if (!cv_.wait_for(lock, std::chrono::seconds(10), [this] { return current_capacity_ > 0 || shutting_down_; })) return false;
            }

            if (shutting_down_ && current_capacity_ == 0) return false;

            --current_capacity_;
            out = buffer_[pop_ptr_];
            pop_ptr_ = (pop_ptr_ + 1) % BUFFER_SIZE;
            return true;
        }        

        int getDropCounter() {
            return drop_counter_;
        }

        void shutdown() {
            {
                std::lock_guard<std::mutex> lock(mtx_);
                shutting_down_ = true;
            }
            cv_.notify_all();
        }

    private:
        std::array<LogRecord, 1024> buffer_;
        const int BUFFER_SIZE = 1024;
        int pop_ptr_;
        int push_ptr_;
        int current_capacity_;
        int drop_counter_;
        bool shutting_down_;
        std::mutex mtx_;
        std::condition_variable cv_;
};
