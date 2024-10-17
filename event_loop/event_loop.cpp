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

// Just a support function for displaying chosen events.
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

// Thanks Dr. Hank!
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

// Need a reference to the atomic bool to allow for external signaling.
int task(int start_value, std::atomic_bool& is_canceled){
    spdlog::info("Task starting!\n");
    int task_counter = start_value;
    while(!is_canceled and task_counter < 1000){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (task_counter % 100 == 0){
            spdlog::info("Task running... task_counter = {}", task_counter);
        }
        task_counter++;
    }
    return task_counter;
}

int main(){
    std::mt19937_64 mt { 42 };  // Create the random number engine with a seed
    std::uniform_int_distribution<int> rand_int {10, 100};

    std::atomic_bool task_cancelled { false };
    // Used if you want to control a thread yourself.
    // std::thread task_thread;
    // Used to get the result using a future.
    std::future<int> task_result;
    spdlog::info("Let's pretend! Event loop!");
    
    View ui {};

    while (true){
        // A text UI to imitate getting an event.
        ui.prompt();
        auto input_event = ui.get_input();
        spdlog::info("User entered {}", event_string(input_event));
        // Process the incoming event
        if (input_event == Event::QUIT){
            break;
        }
        else if (input_event == Event::START){
            task_cancelled = false;
            // Raw thread implementation - cannot get result easily.
            // std::thread new_task {task, rand_int(mt), std::ref(task_cancelled)};
            // task_thread = std::move(new_task);

            // Lean on async to get a future to get results.
            task_result = std::async(
                std::launch::async,
                task,
                rand_int(mt),
                std::ref(task_cancelled)
            );
        }
        else if (input_event == Event::STOP){
            task_cancelled = true;
            auto result = task_result.get();
            spdlog::warn("Stopped task early. result = {}", result);
            // task_thread.join();
        }
        else if (input_event == Event::COMPLETE){
            spdlog::info("Waiting for completion...");
            task_result.wait();
            auto result = task_result.get();
            spdlog::info("Done! result = {}", result);
            // task_thread.join();
            break;
        }
    }

}