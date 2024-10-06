#include <thread>
#include <sstream>
#include <string>
#include "spdlog/spdlog.h"
#include "utils/utils.hpp"

// A function to say hello! Assumed to be run in a thread.
void hello(){
    auto id = std::this_thread::get_id();
    spdlog::info("Hello world from thread {}", id_to_str(id));
}

int main(){
    auto main_id = std::this_thread::get_id();
    spdlog::info("Main thread {}", id_to_str(main_id));

    std::thread hello_thread {hello};
    hello_thread.join();
}