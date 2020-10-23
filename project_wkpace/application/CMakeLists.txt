# Cmake version require
cmake_minimum_required(VERSION 3.13.1)

add_subdirectory_ifdef(CONFIG_GUIX demo_guix_smart_watch)

# Source files
target_sources(app 
    PRIVATE 
#    gpio_unit_test.c
)
