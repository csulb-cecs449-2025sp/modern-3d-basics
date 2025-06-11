add_executable (Shaders "src/main.cpp"  "include/ShaderProgram.h" "src/ShaderProgram.cpp" )

# Find and link external libraries, like SFML.
# This only works if Vcpkg has been configured correctly.
find_package(SFML COMPONENTS System Window Graphics CONFIG REQUIRED)
target_link_libraries(Shaders PRIVATE SFML::System SFML::Window SFML::Graphics)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(Shaders PRIVATE assimp::assimp)

find_package(glad CONFIG REQUIRED)
target_link_libraries(Shaders PRIVATE glad::glad)

target_include_directories(Shaders PUBLIC "./include")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Shaders PROPERTY CXX_STANDARD 20)
endif()

add_custom_target(copyshaders_shaders
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders_source ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMENT "copying ${CMAKE_SOURCE_DIR}/shaders_source to ${CMAKE_CURRENT_BINARY_DIR}/shaders"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_dependencies(Shaders copyshaders_shaders)