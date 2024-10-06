add_executable(hello_concurrency
    hello_concurrency.cpp
)

target_link_libraries(hello_concurrency
    spdlog::spdlog
)