target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET publicHeaders
    TYPE HEADERS
    FILES
    Grid.h
    Node.h
    ConductorElement.h
    vec3.h
)
