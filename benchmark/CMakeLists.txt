find_package(benchmark REQUIRED)

add_executable(benchmark_${PROJECT_NAME} test_ecs.cpp)
target_link_libraries(benchmark_${PROJECT_NAME}
        PRIVATE
        ${PROJECT_NAME}
        benchmark::benchmark)
