# Add library definitions here
# See README.md for CMake library patterns and examples
target_add_executable(qt_test)
target_include_directories(qt_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_sources(qt_test PRIVATE GLWidget.cpp InstancedCubes.cpp Renderer.cpp main.cpp
        ${CMAKE_SOURCE_DIR}/include/QtRendering/GLWidget.hpp)
target_link_libraries(qt_test PRIVATE
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        OpenGL::GL
        glm::glm
        glad::glad)
target_compile_definitions(qt_test PRIVATE VERT_SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders/vert.glsl"
FRAG_SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders/frag.glsl")