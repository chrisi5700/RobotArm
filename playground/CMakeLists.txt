
target_add_executable(qt_triangle
        main.cpp
)

target_link_libraries(qt_triangle PRIVATE
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        OpenGL::GL
        glm::glm
        glad::glad
        glfw
)