find_package(VTK REQUIRED COMPONENTS
    CommonCore
    CommonDataModel
    IOXML
    # IOHDF
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

target_sources(${PROJECT_NAME} PRIVATE
    CalcMesh.cpp
    CalcNode.cpp
    ConductorElement.cpp
    vector3D.cpp
)
