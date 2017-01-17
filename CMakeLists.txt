project(teletest)
cmake_minimum_required(VERSION 2.8)

# options
option(DOWNLOAD_GTEST "DOWNLOAD_GTEST" OFF)
option(INCLUDE_GTESTS "INCLUDE_GTESTS" ON)

option(DOWNLOAD_SERIALPORT "DOWNLOAD_LIBSERIAL" OFF)

# c++11
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# debug symbols
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")



enable_testing()
include(ExternalProject)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

# gtest
IF (${INCLUDE_GTEST})
    IF (${DOWNLOAD_GTEST})

        ExternalProject_Add(googletest
            GIT_REPOSITORY https://github.com/google/googletest
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION})

        include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
        link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
    ELSEIF (${DOWNLOAD_GTEST})
        find_package(GTest GMock REQUIRED)
    ENDIF (${DOWNLOAD_GTEST})
ENDIF (${INCLUDE_GTEST})

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
    ${CMAKE_CURRENT_SOURCE_DIR}/misc
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS})

set(SOURCES_CPP
    src/ConfigManager.cpp
    src/CrcUtil.cpp
    src/SerialInterface.cpp
    src/TeleAssertion.cpp
)

set(SOURCES_C
    misc/crc.c
    misc/serial.c
    misc/teletest.c)

add_executable(${PROJECT_NAME}
    src/main.cpp
    ${SOURCES_CPP})
target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} serialport inih)

#TODO add if case
add_executable(uut_example misc/uut_example.c ${SOURCES_C})
#target_link_libraries(uut_example serialport wiringPi)

add_executable(crc_example misc/crc_example.c)
#target_link_libraries(crc_example)

add_executable(foo misc/foo.cpp)

# testing
IF (${INCLUDE_GTESTS})
    include_directories(misc)

    add_executable(test_uc test/test_uc.cpp ${SOURCES_C})
    target_link_libraries(test_uc gtest gtest_main)
    #add_test(unit_test test_uc)

    add_executable(test_pc_client test/test_pc_client.cpp ${SOURCES_CPP})
    target_link_libraries(test_pc_client gtest gtest_main inih serialport)
    #add_test(unit_test test_pc_client)


ENDIF (${INCLUDE_GTESTS})
