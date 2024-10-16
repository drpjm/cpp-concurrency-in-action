add_executable(pipeline_async
    pipeline_async.cpp
)

target_include_directories(pipeline_async
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(pipeline_async
    spdlog::spdlog
)