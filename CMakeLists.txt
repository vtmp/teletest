project(teletest)
cmake_minimum_required(VERSION 2.8)

# c++11
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

# gtest
enable_testing()
find_package(GTest REQUIRED)

#message( ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS})

set(SOURCES
    src/main.cpp
    src/TeleAssertion.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

# testing
#add_test(AllTestsInFoo foo)
