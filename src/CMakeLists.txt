# Add library definitions here
# See README.md for CMake library patterns and examples
if(EMSCRIPTEN)
    qt_add_executable(robot_arm)
    target_link_options(robot_arm PRIVATE
            -sALLOW_MEMORY_GROWTH=1
            "--preload-file=${CMAKE_SOURCE_DIR}/shaders@/shaders"
    )
    target_compile_definitions(robot_arm PRIVATE SHADER_PATH="/shaders/")
else()
    target_add_executable(robot_arm)
    target_compile_definitions(robot_arm PRIVATE SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders")
endif()
target_include_directories(robot_arm PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_sources(robot_arm PRIVATE Scene.cpp main.cpp RobotArm.cpp RobotArmControls.cpp
        InstancedObject.cpp GLCommon.cpp ShaderControls.cpp Camera.cpp GLWindow.cpp
        ${CMAKE_SOURCE_DIR}/include/QtRendering/GLWindow.hpp
        ${CMAKE_SOURCE_DIR}/include/QtRendering/RobotArmControls.hpp
        ${CMAKE_SOURCE_DIR}/include/QtRendering/ShaderControls.hpp
)
target_link_libraries(robot_arm PRIVATE
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        glm::glm
        ${GL_LIBS})
# target_compile_definitions(qt_test PRIVATE SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders/")