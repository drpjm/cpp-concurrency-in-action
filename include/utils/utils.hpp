#include <thread>
#include <string>
#include <sstream>

// A simple utility to conver the id to a printable string.
std::string id_to_str(std::thread::id id){
    std::ostringstream out_str;
    out_str << id;
    return out_str.str();
}