add_executable(hello_concurrency
    hello_concurrency.cpp
)

target_include_directories(hello_concurrency
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(hello_concurrency
    spdlog::spdlog
)