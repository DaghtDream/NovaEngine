# ----------------- NovaEngine 测试程序配置 -----------------

#创建测试可执行文件
add_executable(NovaEngine_Tests
 "main_test.cpp"  # 测试程序入口文件
"TestFramework.h"
 "SceneTest/MockCapability.h"
 "SceneTest/MockCapability2.h"
 "SceneTest/Test_GameObject.cpp"
 "SceneTest/Test_Component.cpp"
 "SceneTest/Test_Capabilities.cpp"
 "SceneTest/Test_GameObject.h"
 "SceneTest/Test_Component.h"
 "SceneTest/Test_Capabilities.h"
 "SceneTest/MockCapability3.h"
 "SceneTest/Integration_Test.h"
 "SceneTest/Integration_Test.cpp"
 "SceneTest/stressTest_1.h"
 "SceneTest/stressTest_1.cpp"
 "SceneTest/stressTest_2.h"
 "SceneTest/stressTest_2.cpp"
 "SceneTest/stressTest_3.h"
 "SceneTest/stressTest_3.cpp"
 "SceneTest/CameraTest/CameraTest1.h"
 "SceneTest/CameraTest/CameraTest1.cpp")

target_include_directories(NovaEngine_Tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#链接引擎核心库（自动继承 include 路径、GLFW、OpenGL 和编译定义）
target_link_libraries(NovaEngine_Tests PRIVATE NovaEngine_Core)


#开启 Release 模式下的全程序优化
if(lto_supported)
    set_property(TARGET NovaEngine_Tests PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
endif()

set_target_properties(NovaEngine_Tests PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

add_custom_command(TARGET NovaEngine_Tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shader"
        "$<TARGET_FILE_DIR:NovaEngine_Tests>/shader"
)