find_package(VTK REQUIRED COMPONENTS
    CommonCore
    FiltersCore
    CommonDataModel
    IOXML
    # IOHDF
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

target_sources(${PROJECT_NAME} PRIVATE
    Grid.cpp
    ConductorElement.cpp
    vec3.cpp
)
