set(EXAMPLES
    dipole.cpp
    plane.cpp
    ring.cpp
    solenoid.cpp
)

foreach(EXAMPLE_FILE IN LISTS EXAMPLES)
    get_filename_component(EXAMPLE_FILENAME ${EXAMPLE_FILE} NAME_WE)
    set(EXECUTABLE_NAME ${EXAMPLE_FILENAME})
    add_executable(${EXECUTABLE_NAME} ${EXAMPLE_FILE})
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${PROJECT_NAME})
    message("EXAMPLE: ${EXAMPLE_FILE}")
endforeach()
