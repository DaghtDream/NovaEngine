cmake_minimum_required(VERSION 3.10)
project(3DEditor)

set(CMAKE_CXX_STANDARD 11)

set(GLFW_DIR "F:/3DEditor_gitee/glfw-3.4/build/src/Debug")
set(GLFW_LIBRARY "${GLFW_DIR}/glfw3.lib")

set(GLAD_DIR "F:/3DEditor_gitee/glad")  
include_directories("${GLAD_DIR}/include") 

include_directories("F:/3DEditor_gitee/glfw-3.4/include")  

find_package(OpenGL REQUIRED)

add_executable(MyOpenGLApp 
    main.cpp
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)  

target_sources(MyOpenGLApp PRIVATE "${GLAD_DIR}/src/glad.c")  

target_include_directories(MyOpenGLApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

target_compile_definitions(MyOpenGLApp PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(MyOpenGLApp ${GLFW_LIBRARY} OpenGL::GL)  

