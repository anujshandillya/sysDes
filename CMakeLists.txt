cmake_minimum_required(VERSION 3.10)
project(sysDes)
add_executable(sysDes src/main.cpp)
add_subdirectory(modules/Crow)

target_include_directories(sysDes PUBLIC modules/Crow/include)
target_link_libraries(sysDes PUBLIC Crow)
