cmake_minimum_required(VERSION 3.28)
project(RobotArm)

# Include CMake modules for target helpers and compiler settings
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(TargetHelpers)


find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
if(EMSCRIPTEN)
    message("Configuring WASM")
    set(GL_LIBS "")  # Emscripten links GL automatically
    # Fetch glm for WASM (avoids 32-bit vs 64-bit config issues)
    include(FetchContent)
    FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG 1.0.1
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glm)
    # WASM-specific flags
else()
    message("Configuring Local")
    # Desktop build with vcpkg
    find_package(glad CONFIG REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(glm CONFIG REQUIRED)

    set(GL_LIBS glad::glad OpenGL::GL)
endif()
# find_package(glfw3 CONFIG REQUIRED)
set(CMAKE_AUTOMOC ON)

# Default cache variables (can be overridden by CMakePresets.json or command line)
set(ENABLE_COMPILER_WARNINGS ON CACHE BOOL "Enable compiler warnings")
set(TREAT_WARNINGS_AS_ERRORS OFF CACHE BOOL "Treat warnings as errors")
set(ENABLE_SANITIZER_ADDRESS OFF CACHE BOOL "Enable AddressSanitizer")
set(ENABLE_SANITIZER_THREAD OFF CACHE BOOL "Enable ThreadSanitizer")
set(ENABLE_SANITIZER_UNDEFINED OFF CACHE BOOL "Enable UndefinedBehaviorSanitizer")
set(ENABLE_LTO OFF CACHE BOOL "Enable Link Time Optimization")

add_subdirectory(src)

# add_subdirectory(tests)
# add_subdirectory(libs/SFML) # for example to add other cmake dependencies