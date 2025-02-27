add_executable (VertexBuffers "src/main.cpp")


# Find and link external libraries, like SFML.
# This only works if Vcpkg has been configured correctly.
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(VertexBuffers PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(VertexBuffers PRIVATE assimp::assimp)

find_package(glad CONFIG REQUIRED)
target_link_libraries(VertexBuffers PRIVATE glad::glad)

target_include_directories(VertexBuffers PUBLIC "./include")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VertexBuffers PROPERTY CXX_STANDARD 20)
endif()
