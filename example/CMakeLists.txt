cmake_minimum_required(VERSION 3.5)

# Add this before project()
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../ssd1306/")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ssd1306-example)
