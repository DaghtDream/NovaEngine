cmake_minimum_required(VERSION 3.10)
project(3DEditor)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_DIR "F:/3DEditor_gitee/glfw-3.4/build/src/Debug")
set(GLFW_LIBRARY "${GLFW_DIR}/glfw3.lib")

set(GLAD_DIR "F:/3DEditor_gitee/glad")  
include_directories("${GLAD_DIR}/include") 

include_directories("F:/3DEditor_gitee/glfw-3.4/include")  

find_package(OpenGL REQUIRED)

add_executable(MyOpenGLApp 
    src/main.cpp
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imguizmo/ImGuizmo.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    
    
    
   "include/Utils/Logger.h" "include/UI/GUILogger.h"  
   "include/Scene/DataComponents/Mesh.h" 
   "include/Scene/ICapabilities/ICapabilities.h" 
   "include/Scene/Capabilities/Render.h"
   "include/Scene/ICommand/ICommand.h" "include/Scene/Command/CreateObjectCommand.h" "src/Scene/Command/CreateObjectCommand.cpp" "include/Scene/Command/DestroyObjectCommand.h" "src/Scene/Command/DestroyObjectCommand.cpp" "src/Scene/Core/Scene.cpp")  

target_sources(MyOpenGLApp PRIVATE "${GLAD_DIR}/src/glad.c")  

target_include_directories(MyOpenGLApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo
)

target_include_directories(MyOpenGLApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/glm
)

target_include_directories(MyOpenGLApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Platform
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Rendering
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Scene
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Utils
)


target_compile_definitions(MyOpenGLApp PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(MyOpenGLApp ${GLFW_LIBRARY} OpenGL::GL)  

