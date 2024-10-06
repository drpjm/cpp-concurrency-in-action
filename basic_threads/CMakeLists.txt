add_executable(basic_threads
    basic_threads.cpp
)

target_include_directories(basic_threads
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(basic_threads
    spdlog::spdlog
)