#include <thread>
#include <chrono>
#include <future>
#include <limits>
#include <random>
#include "spdlog/spdlog.h"
#include "utils/utils.hpp"
#include "utils/threadsafe_queue.hpp"
#include "utils/point2d.hpp"

int main(){
    spdlog::info("Creating pipeline tasks...");

    // Queue to connect the main thread to t1
    ThreadsafeQueue<Point2D> q_main_t1 {};
    // Queue to connect t1 to t2
    ThreadsafeQueue<Point2D> q_t1_t2 {};

    std::string t1_name {"t1"};
    auto t1_done = std::async(
        // Capture with a reference
        [&q_main_t1, &q_t1_t2, t1_name] {
            while (true) {
                if(!q_main_t1.empty()){
                    auto point = q_main_t1.wait_and_pop();
                    // The Point2D is wrapped in a shared_ptr, so you need to unpack it as a pointer.
                    auto x_val = point.get()->get_x();
                    auto y_val = point.get()->get_y();
                    spdlog::info("{0} received point ({1},{2})", t1_name, x_val, y_val);
                    if ( (x_val == std::numeric_limits<float>::max()) 
                            && (y_val == std::numeric_limits<float>::max()) ) {
                        spdlog::info("{} Poison pill message! All done.", t1_name);
                        // Pass Poison Pill to the next thread.
                        q_t1_t2.push(Point2D(x_val, y_val));
                        return true;
                    }
                    // Process and pass along to next thread!
                    Point2D new_point { x_val * (float)-1.0, y_val * (float)-1.0 };
                    q_t1_t2.push(new_point);
                }
                else{
                    spdlog::warn("{} Nothing to work on!", t1_name);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
    );

    std::string t2_name {"t2"};
    auto t2_done = std::async(
        [&q_t1_t2, t2_name] {
            while (true) {

                auto point = q_t1_t2.wait_and_pop();
                // The Point2D is wrapped in a shared_ptr, so you need to unpack it as a pointer.
                auto x_val = point.get()->get_x();
                auto y_val = point.get()->get_y();
                spdlog::info("{0} received point ({1},{2})", t2_name, x_val, y_val);
                if ( (x_val == std::numeric_limits<float>::max()) 
                        && (y_val == std::numeric_limits<float>::max()) ) {
                    spdlog::info("{} Poison pill message! All done.", t2_name);
                    return true;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
    );

    bool is_done = false;
    int MAX_DATA = 10;
    int data_count = 0;
    Point2D END_POINT {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};

    // Random numbers for fun!
    std::mt19937_64 mt { 42 };  // Create the random number engine with a seed
    std::uniform_real_distribution<float> rand_real {-5.0, 5.0};    // Creates a uniform distribution
    
    while(!is_done){

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));       
        spdlog::info("Sending in new point...");
        float x { rand_real(mt) };
        float y { rand_real(mt) };
        q_main_t1.push(Point2D(x,y));
        if (data_count > MAX_DATA){
            is_done = true;
            q_main_t1.push(END_POINT);
        }
        data_count++;

    }

    spdlog::info("Checking the futures!");
    spdlog::info("t1 done? {}", t1_done.get());
    spdlog::info("t2 done? {}", t2_done.get());
    
}