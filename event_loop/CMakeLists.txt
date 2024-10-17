add_executable(event_loop
    event_loop.cpp
)

target_include_directories(event_loop
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(event_loop
    spdlog::spdlog
)