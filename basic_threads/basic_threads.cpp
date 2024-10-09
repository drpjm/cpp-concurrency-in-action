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
            spdlog::info("Thread[{0}] - Hello! {1}", id_to_str(std::this_thread::get_id()), iters_);
            iters_++;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms_));
        }
        spdlog::info("Thread[{0}] - max iters({1}), sleep({2}) Task complete!", id_to_str(std::this_thread::get_id()), max_iters_, sleep_ms_);
        return;
    }
};

int main(){
    spdlog::info("Creating new background task...");
    BackgroundTask back_task1 {10, 1000};
    BackgroundTask back_task2 {15, 500};

    std::thread task_thread1 {back_task1};
    std::thread task_thread2 {back_task2};
    task_thread1.detach();
    task_thread2.detach();

    spdlog::info("Can we join with thread1? {0}", task_thread1.joinable());
    spdlog::info("Can we join with thread2? {0}", task_thread2.joinable());

    bool is_done = false;
    int count = 0;
    int MAX_COUNT = 15;
    while (!is_done) {
        auto curr_thread_name = id_to_str(std::this_thread::get_id());
        spdlog::info("Main thread {} running.", curr_thread_name);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (count > MAX_COUNT){
            is_done = true;
        }
        count += 1;   
    }
}