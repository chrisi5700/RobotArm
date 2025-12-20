# Add library definitions here
# See README.md for CMake library patterns and examples
if(EMSCRIPTEN)
    qt_add_executable(qt_test)
    target_link_options(qt_test PRIVATE
            -sALLOW_MEMORY_GROWTH=1
            "--preload-file=${CMAKE_SOURCE_DIR}/shaders@/shaders"
    )
    target_compile_definitions(qt_test PRIVATE SHADER_PATH="/shaders/")
else()
    target_add_executable(qt_test)
    target_compile_definitions(qt_test PRIVATE SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders")
endif()
target_include_directories(qt_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_sources(qt_test PRIVATE Scene.cpp main.cpp RobotArm.cpp RobotArmControls.cpp
        InstancedObject.cpp GLCommon.cpp ShaderControls.cpp Camera.cpp GLWindow.cpp
        ${CMAKE_SOURCE_DIR}/include/QtRendering/GLWindow.hpp
        ${CMAKE_SOURCE_DIR}/include/QtRendering/RobotArmControls.hpp
        ${CMAKE_SOURCE_DIR}/include/QtRendering/ShaderControls.hpp
)
target_link_libraries(qt_test PRIVATE
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        glm::glm
        ${GL_LIBS})
# target_compile_definitions(qt_test PRIVATE SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders/")