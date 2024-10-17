#include <thread>
#include <chrono>
#include <future>
#include <limits>
#include <random>
#include <iostream>
#include "spdlog/spdlog.h"
#include "utils/utils.hpp"
#include "utils/threadsafe_queue.hpp"

enum class Event {
    START,
    STOP,
    COMPLETE,
    QUIT,
    UNKNOWN
};

std::string event_string(Event event){
    switch (event)
    {
    case Event::START :
        return "start";
    case Event::STOP :
        return "stop";
    case Event::COMPLETE :
        return "complete";
    case Event::QUIT :
        return "quit";
    default:
        return "?";
    }
}

// Thanks to Dr. Hank!
class View {
    public:
        void prompt(){
            std::cout << "----" << std::endl;
            std::cout << "Hello - enter your command (start, stop, complete, quit)" << ": ";
        }
        Event get_input(){
            std::string command;
            std::cin >> command;
            std::cout << "----" << std::endl;
            if (command == "start"){
                return Event::START;
            }
            else if (command == "stop"){
                return Event::STOP;
            }
            else if (command == "complete"){
                return Event::COMPLETE;
            }
            else if (command == "quit"){
                return Event::QUIT;
            }
            else {
                return Event::UNKNOWN;
            }
        }
};

void task(std::atomic<bool> is_canceled){
    spdlog::info("Task starting!\n");
    while(!is_canceled){
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

int main(){
    std::atomic<bool> task_cancelled { false };
    std::thread task_thread;
    spdlog::info("Let's pretend! Event loop!");
    View ui {};

    while (true){
        // A text UI to imitate getting an event.
        ui.prompt();
        auto input_event = ui.get_input();
        spdlog::info("Received {}", event_string(input_event));
        // Process the incoming event
        if (input_event == Event::QUIT){
            break;
        }
        else if (input_event == Event::START){
            task_cancelled = false;
            std::thread new_task {task, std::ref(task_cancelled)};
            // task_thread = std::move(new_task);
        }
        else if (input_event == Event::STOP){
            task_cancelled = true;
            task_thread.join();
        }
        else if (input_event == Event::COMPLETE){
            task_thread.join();
            // display_results();
            break;
        }
    }

}