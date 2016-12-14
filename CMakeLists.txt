project(teletest)
cmake_minimum_required(VERSION 2.8)

# c++11
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

# debug symbols
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# gtest
enable_testing()
find_package(GTest REQUIRED)

# libserialport
find_library(LIBSERIALPORT_LIBRARIES serialport)

#message( ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS})

set(SOURCES
    src/main.cpp
    src/SerialInterface.cpp
    src/TeleAssertion.cpp
    src/FunctionInfo.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} serialport)

#TODO add if case
add_executable(uut_example misc/uut_example.c)
target_link_libraries(uut_example serialport wiringPi)

# testing
#add_test(AllTestsInFoo foo)

add_executable(crc_example misc/crc_example.c)
#target_link_libraries(crc_example)
