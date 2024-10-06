#include <thread>
#include <chrono>
#include "spdlog/spdlog.h"
#include "utils/utils.hpp"

class BackgroundTask {

public:
    explicit BackgroundTask(int max_iters, int sleep_ms) : max_iters_ (max_iters), sleep_ms_ (sleep_ms) {};
    void operator()() {
        say_something();
    }

private:
    int iters_ = 0;
    int max_iters_;
    int sleep_ms_;
    void say_something(){
        while (iters_ <= max_iters_){
            spdlog::info("Hello! {}", iters_);
            iters_++;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms_));
        }
        spdlog::info("max iters({}) Task complete!", max_iters_);
        return;
    }
};

int main(){
    spdlog::info("Creating new background task...");
    BackgroundTask back_task1 {10, 600};
    BackgroundTask back_task2 {13, 325};

    std::thread task_thread1 {back_task1};
    std::thread task_thread2 {back_task2};
    task_thread1.join();
    task_thread2.join();
}