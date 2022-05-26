cmake_minimum_required(VERSION 3.5)

project(client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

execute_process(COMMAND "bash" "-c" "conan install ${CMAKE_SOURCE_DIR} -of ${CMAKE_BINARY_DIR} --build=missing"  RESULT_VARIABLE CMD_RESULT OUTPUT_FILE CMD_OUTPUT)
message(STATUS "conan install result: " ${CMD_RESULT})
message(STATUS "conan install output: ${CMD_OUTPUT}")

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)


find_package(Boost REQUIRED)
find_package(spdlog REQUIRED)

set(SOURCES
    src/main.cpp
    src/client/client.cpp)

set(HEADERS
    src/client/client.h
    src/application.h
    src/randomizer.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} Boost::boost spdlog::spdlog)
