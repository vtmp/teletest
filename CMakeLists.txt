project(teletest)
cmake_minimum_required(VERSION 2.8)

# options
option(DOWNLOAD_GTEST "DOWNLOAD_GTEST" OFF)
option(DOWNLOAD_SERIALPORT "DOWNLOAD_LIBSERIAL" OFF)

# c++11
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

# debug symbols
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")



enable_testing()
include(ExternalProject)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

# gtest
IF (${DOWNLOAD_GTEST})

    ExternalProject_Add(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION})

    include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
    link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
ENDIF (${DOWNLOAD_GTEST})

# libserialport
IF (${DOWNLOAD_SERIALPORT})

    # TODO FIXMEload externally
    ExternalProject_Add(serialport_tmp
        GIT_REPOSITORY git://sigrok.org/libserialport
        GIT_TAG master
        UPDATE_COMMAND ../${EP_BASE}/serialport_tmp/autogen.sh
        CONFIGURE_COMMAND ../${EP_BASE}/serialport_tmp/configure --prefix=${EXTERNAL_INSTALL_LOCATION}
        BUILD_COMMAND make
        INSTALL_COMMAND make install)


    add_library(serialport SHARED IMPORTED)
    add_dependencies(serialport serialport_tmp)

ELSE (${DOWNLOAD_SERIALPORT})
    find_library(LIBSERIALPORT_LIBRARIES serialport)
ENDIF (${DOWNLOAD_SERIALPORT})


# inih
ExternalProject_Add(inih_tmp
    GIT_REPOSITORY https://github.com/benhoyt/inih.git
    GIT_TAG master
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")

ExternalProject_Get_Property(inih_tmp SOURCE_DIR)
message(${SOURCE_DIR})
# TODO FIXME this is not working properly
include_directories(${SOURCE_DIR}/cpp)
include_directories(${SOURCE_DIR})
add_library(inih
    ${SOURCE_DIR}/cpp/INIReader.cpp
    ${SOURCE_DIR}/ini.c)

find_library(INIH inih)

#message( ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS})

set(SOURCES
    src/main.cpp
    src/ConfigManager.cpp
    src/SerialInterface.cpp
    src/TeleAssertion.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} serialport inih)

#TODO add if case
#add_executable(uut_example misc/uut_example.c)
#target_link_libraries(uut_example serialport wiringPi)

# testing
#add_test(AllTestsInFoo foo)

add_executable(crc_example misc/crc_example.c)
#target_link_libraries(crc_example)
