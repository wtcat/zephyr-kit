# Application

if (CONFIG_GUIX)
if (CONFIG_WATCH_FACE_TEST)
    add_subdirectory(watch_face)
else()
    add_subdirectory(demo_guix_smart_watch)
endif()
endif(CONFIG_GUIX)
