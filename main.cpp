#include "logger.h"
#include "logrecord.h"
#include "ringbuffer.h"
#include "logger_registry.h"
#include "dispatcher.h"
#include "producer.h"
#include <iostream>
#include <chrono>

const int RUNTIME_IN_SECONDS = 20;

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    Producer p1, p2, p3, p4;
    Dispatcher d;
    std::jthread producer_thread1([&p1](std::stop_token st) { p1.generateLog(st); });
    std::jthread producer_thread2([&p2](std::stop_token st) { p2.generateLog(st); });
    std::jthread producer_thread3([&p3](std::stop_token st) { p3.generateLog(st); });
    std::jthread producer_thread4([&p4](std::stop_token st) { p4.generateLog(st); });
    std::jthread dispatcher_thread([&d](std::stop_token st) { d.dispatch(st); });

    auto& buffer = LoggerRegistry::getInstance().getBuffer();

    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = current_time - start_time;
    
        if (elapsed_time.count() >= RUNTIME_IN_SECONDS) {
            producer_thread1.request_stop();
            producer_thread2.request_stop();
            producer_thread3.request_stop();
            producer_thread4.request_stop();
            dispatcher_thread.request_stop();
            buffer.shutdown();
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int drop_count = buffer.getDropCounter();
    
    std::cout << "------------------------------------------\n";
    std::cout << "Drop count is: " << drop_count << std::endl;
}