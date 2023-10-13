# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

#extra dirs that are not standard for ESP-IDF
set(EXTRA_COMPONENT_DIRS Libs)
# Add include directories to the build
include_directories(Libs
                    Libs/Header/Core 
                    Libs/Header/Drivers )

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(FlashTestS2)
