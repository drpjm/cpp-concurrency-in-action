#include <thread>
#include <sstream>
#include <string>
#include "spdlog/spdlog.h"

// A simple utility to conver the id to a printable string.
std::string id_to_str(std::thread::id id){
    std::ostringstream out_str;
    out_str << id;
    return out_str.str();
}

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