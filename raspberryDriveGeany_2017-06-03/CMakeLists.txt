cmake_minimum_required (VERSION 2.8)
project (mainDrive CXX)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" )

# Add sources here... #
add_executable (mainDrive mainDrive.cpp Tastatur.h Tastatur.cpp CamPixy.h CamPixy.cpp Bildanalyse.h Bildanalyse.cpp Regler.h Regler.cpp)

# libpixyusb should always come before libboost and libusb #
target_link_libraries (mainDrive pixyusb)
target_link_libraries (mainDrive wiringPi)

find_package ( libpixyusb REQUIRED )
find_package ( libusb-1.0 REQUIRED )
find_package ( Boost 1.49 COMPONENTS thread system chrono REQUIRED)

target_link_libraries (mainDrive ${Boost_LIBRARIES})
target_link_libraries (mainDrive ${LIBUSB_1_LIBRARY})

file(STRINGS "cmake/VERSION" LIBPIXY_VERSION)
add_definitions(-D__LIBPIXY_VERSION__="${LIBPIXY_VERSION}")

include_directories (src
                     include
                     ../../common
                     ${Boost_INCLUDE_DIR}
                     ${LIBUSB_1_INCLUDE_DIRS})

