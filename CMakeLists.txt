cmake_minimum_required(VERSION 3.16)
project(DroneRacingLite VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# Find Bullet Physics
find_package(Bullet REQUIRED COMPONENTS Dynamics Collision LinearMath)

# Find GLM (header-only)
find_package(glm REQUIRED)

# Include directories
include_directories(include)
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${BULLET_INCLUDE_DIRS})

# Set language for glad.c
set_source_files_properties(src/glad.c PROPERTIES LANGUAGE C)

# Add executable
add_executable(drone-sim
    src/main.cpp
    src/glad.c
    src/renderer/renderer.cpp
    src/physics/physics.cpp
    src/physics/debug_drawer.cpp
    src/controls/controls.cpp
    src/mission/mission.cpp
)

# Link libraries
target_link_libraries(drone-sim
    OpenGL::GL
    glfw
    ${BULLET_LIBRARIES}
)