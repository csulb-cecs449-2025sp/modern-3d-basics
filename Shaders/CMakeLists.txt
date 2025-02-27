add_executable (Shaders "src/main.cpp"  "include/ShaderProgram.h" "src/ShaderProgram.cpp" )

# Find and link external libraries, like SFML.
# This only works if Vcpkg has been configured correctly.
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(Shaders PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(Shaders PRIVATE assimp::assimp)

find_package(glad CONFIG REQUIRED)
target_link_libraries(Shaders PRIVATE glad::glad)

target_include_directories(Shaders PUBLIC "./include")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Shaders PROPERTY CXX_STANDARD 20)
endif()