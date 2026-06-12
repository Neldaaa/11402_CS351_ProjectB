cmake_minimum_required(VERSION 3.10)
project(CSV_Mini_Database VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Main application executable
add_executable(csv_db src/main.cpp)

# Testing configuration
enable_testing()

add_executable(run_tests tests/test_cases.cpp)
add_test(NAME BasicTests COMMAND run_tests)