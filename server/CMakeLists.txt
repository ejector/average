cmake_minimum_required(VERSION 3.5)

project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

execute_process(COMMAND "bash" "-c" "conan install ${CMAKE_SOURCE_DIR} -of ${CMAKE_BINARY_DIR} --build=missing"  RESULT_VARIABLE CMD_RESULT OUTPUT_FILE CMD_OUTPUT)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Boost REQUIRED)
find_package(spdlog REQUIRED)

set(SOURCES
    src/main.cpp)

set(HEADERS
    src/randomstorage.h
    src/server.h
    src/application.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} Boost::boost spdlog::spdlog)
