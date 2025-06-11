add_executable (Projections "src/main.cpp" "include/ShaderProgram.h" "src/ShaderProgram.cpp" )


# Find and link external libraries, like SFML.
# This only works if Vcpkg has been configured correctly.
find_package(SFML COMPONENTS System Window Graphics CONFIG REQUIRED)
target_link_libraries(Projections PRIVATE SFML::System SFML::Window SFML::Graphics)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(Projections PRIVATE assimp::assimp)

find_package(glad CONFIG REQUIRED)
target_link_libraries(Projections PRIVATE glad::glad)

target_include_directories(Projections PUBLIC "./include")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Projections PROPERTY CXX_STANDARD 20)
endif()

add_custom_target(copyshaders_projections
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders_source ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMENT "copying ${CMAKE_SOURCE_DIR}/shaders_source to ${CMAKE_CURRENT_BINARY_DIR}/shaders"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_dependencies(Projections copyshaders_projections)

add_custom_target(copymodels_projections
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/models
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/models ${CMAKE_CURRENT_BINARY_DIR}/models
        COMMENT "copying ${CMAKE_SOURCE_DIR}/models to ${CMAKE_CURRENT_BINARY_DIR}/models"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_dependencies(Projections copymodels_projections)