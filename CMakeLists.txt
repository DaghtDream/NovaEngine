cmake_minimum_required(VERSION 3.9)
project(NovaEngine)



# ----------------- 配置 /O2 优化 -----------------
if(MSVC)
    # 确保 Release 模式下严格使用 /O2 (优选速度) 和 /Ob2 (适用于任何合适函数的内联扩展)
    add_compile_options("$<$<CONFIG:Release>:/O2;/Ob2>")
endif()

# ----------------- 配置全程序优化 (WPO/LTO) -----------------
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT lto_error)

if(lto_supported)
    message(STATUS "NovaEngine: IPO/LTO (WPO) is supported and ENABLED.")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
else()
    message(WARNING "NovaEngine: IPO/LTO (WPO) is NOT supported: ${lto_error}")
endif()

set(CMAKE_CXX_STANDARD 17)

# ----------------- 第三方依赖库配置 -----------------
set(GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4/build/src/Debug")
set(GLFW_LIBRARY "${GLFW_DIR}/glfw3.lib")

set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glad")  
include_directories("${GLAD_DIR}/include") 
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4/include")  

find_package(OpenGL REQUIRED)

# ----------------- 抽象引擎核心静态库 (NovaEngine_Core) -----------------
add_library(NovaEngine_Core STATIC
    "include/Utils/Logger.h" 
    "include/UI/GUILogger.h"  
    "include/Scene/DataComponents/Mesh.h" 
    "include/Scene/ICapabilities/ICapabilities.h" 
    "include/Scene/Capabilities/Render.h"
    "include/Scene/ICommand/ICommand.h" 
    "include/Scene/Command/CreateObjectCommand.h" 
    "include/Scene/Command/DestroyObjectCommand.h" 
    "include/Scene/DataComponents/Lighting/PointLight.h"
    "include/Rendering/GPUPointLight.h"
    "include/Scene/Core/SceneContants.h"
    "include/Rendering/MeshFactory.h"
    "include/Scene/Capabilities/ColorBlink.h"
    "include/Core/Tool/ProfileBlock.h"
    "include/Rendering/Shader.h"
    "include/Rendering/ReadingFile.h"
    "include/Scene/DataComponents/Camera.h"
    
    "src/Scene/Command/CreateObjectCommand.cpp" 
    "src/Scene/Command/DestroyObjectCommand.cpp" 
    "src/Scene/Core/Scene.cpp"
    "src/Scene/Capabilities/Render.cpp"
    "src/Rendering/MeshFactory.cpp"
    "imgui/imgui.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "imguizmo/ImGuizmo.cpp"
    "imgui/backends/imgui_impl_glfw.cpp"
    "imgui/backends/imgui_impl_opengl3.cpp"
    "src/Scene/Capabilities/ColorBlink.cpp"
    "src/Rendering/Shader.cpp"
    "src/Rendering/ReadingFile.cpp"
    "src/Scene/DataComponents/Camera.cpp" "include/Scene/Capabilities/CameraController.h" "src/Scene/Capabilities/CameraController.cpp")

target_sources(NovaEngine_Core PRIVATE "${GLAD_DIR}/src/glad.c")

# 配置核心库的头文件路径
target_include_directories(NovaEngine_Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo
    ${CMAKE_CURRENT_SOURCE_DIR}/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Platform
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Rendering
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Scene
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Utils
)

target_compile_definitions(NovaEngine_Core PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_compile_definitions(NovaEngine_Core PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD GLFW_INCLUDE_NONE)
target_link_libraries(NovaEngine_Core PUBLIC ${GLFW_LIBRARY} OpenGL::GL)

# ----------------- 主程序目标 (NovaEngine) -----------------
add_executable(NovaEngine "src/NovaEngine main.cpp")

# =====================================================================
# 1. 统一所有 Target（主程序和测试程序）的 VS 调试工作目录为项目源码根目录
# =====================================================================
set_target_properties(NovaEngine PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)



# =====================================================================
# 2. 自动把 shader 资源文件夹复制到构建目录（供直接双击 .exe 时使用）
# =====================================================================
add_custom_command(TARGET NovaEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shader"
        "$<TARGET_FILE_DIR:NovaEngine>/shader"
)


target_link_libraries(NovaEngine PRIVATE NovaEngine_Core)

# ----------------- 测试子目录 -----------------
option(BUILD_NOVA_TESTS "Build NovaEngine Tests" ON)
if(BUILD_NOVA_TESTS)
    add_subdirectory(test)
endif()

