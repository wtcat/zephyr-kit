# Cmake version require
cmake_minimum_required(VERSION 3.13.1)

# Select target board(optional)
set(BOARD apollo3p)

# Modules path
include(cmake/module.cmake)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Add debugger command (optional)
include(cmake/debugger.cmake)

# Project name
project(watch_4g)

#zephyr_include_directories(
#    ../components
#)

# This is ugly!!!!!!!
target_sources(app PRIVATE empty.c)

# Source files
add_subdirectory(application)
