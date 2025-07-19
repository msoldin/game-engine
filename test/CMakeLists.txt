# Prevent GoogleTest from overriding compiler options
set(CMAKE_CTEST_ARGUMENTS "--parallel 4")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Collect all test files in the test directory (recursively)
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# Add the test executable with all collected test files
add_executable(test_${PROJECT_NAME} ${TEST_SOURCES})

# Link GoogleTest to the test executable
target_link_libraries(test_${PROJECT_NAME}
        PRIVATE
        ${PROJECT_NAME}
        GTest::gtest
        GTest::gtest_main
)

# Add the tests
add_test(NAME test_${PROJECT_NAME} COMMAND test_${PROJECT_NAME})