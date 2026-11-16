# NovaEngine - Scene模块测试前置工作步骤清单

> 路径：带渲染的完整压测

## 依赖关系概览

```
Render.h → Render.cpp → CMakeLists.txt → main.cpp → 测试代码
MockCapability.h ────────────────────────────────→ 测试代码
```

---

## 文件 1：`include/Scene/Capabilities/Render.h`（修改现有文件）

**状态：** 当前为空壳，仅继承了 `ICapabilities`

**补全内容：**
- 声明着色器程序句柄 `m_ShaderProgram`
- 声明 uniform location（`uModel` 矩阵、`uColor` 颜色）
- 声明私有方法 `CompileShader`、`CreateShaderProgram`
- 声明 `OnAttach` / `OnDetach` / `OnUpdate` 的 override
- 声明静态方法 `CreateQuadMesh(float width, float height)`
- 新增 include：`glad/glad.h`、`glm.hpp`、`gtc/matrix_transform.hpp`、`gtc/type_ptr.hpp`、`DataComponents/Mesh.h`

**依赖：** 无

---

## 文件 2：`src/Scene/Capabilities/Render.cpp`（新建文件）

**状态：** 不存在，需创建（目录 `src/Scene/Capabilities` 也不存在，需新建）

**实现内容：**

### [1] 硬编码顶点着色器（从 main.cpp 第68-73行迁移）
```glsl
layout(location=0) in vec3 aPos
uniform mat4 uModel
gl_Position = uModel * vec4(aPos, 1.0)
```

### [2] 硬编码片元着色器（从 main.cpp 第91-97行迁移）
```glsl
out vec4 FragColor
uniform vec4 uColor
FragColor = uColor
```

### [3] `CompileShader(unsigned int type, const char* source)`
- 封装 `glCreateShader` / `glShaderSource` / `glCompileShader`
- 编译失败时打印错误日志

### [4] `CreateShaderProgram(const char* vertexSource, const char* fragmentSource)`
- 封装 `glCreateProgram` / `glAttachShader` / `glLinkProgram`
- 链接后 `glDeleteShader` 释放中间着色器
- 链接失败时打印错误日志

### [5] `OnAttach(Scene* scene)`
- 保存 Scene 指针
- 调用 `CreateShaderProgram` 编译着色器
- 获取 `uModel` 和 `uColor` 的 uniform location

### [6] `OnUpdate(float dt)`
- `glClear(GL_COLOR_BUFFER_BIT)`
- `glUseProgram(m_ShaderProgram)`
- 调用 `m_Scene->ForEach<Mesh>(callback)` 遍历所有 Mesh 组件
- 对每个 Mesh：
  - a. 从 `Scene::GetObject()` 获取 GameObject 的 transform
  - b. 构建 model 矩阵：`translate * rotation(quat) * scale`
  - c. 设置 `uModel` uniform
  - d. 检查 PointLight 组件获取颜色，无则默认白色
  - e. 设置 `uColor` uniform
  - f. `glBindVertexArray` + `glDrawElements`

### [7] `OnDetach()`
- `glDeleteProgram(m_ShaderProgram)`

### [8] `static Mesh CreateQuadMesh(float width, float height)`
- 生成 VAO、VBO、EBO
- 顶点数据：4个顶点（position only），以原点为中心
- 索引数据：6个索引（2个三角形）
- 设置 vertex attribute pointer（location=0, vec3）
- 从 main.cpp 第124-151行迁移

**依赖：** 文件 1（Render.h）完成

---

## 文件 3：`test/MockCapability.h`（新建文件）

**状态：** 不存在，需创建

**实现内容：**
- 继承 `SceneManager::ICapabilities`
- 构造函数接收 `int priority`、`const char* name`
- 实现 `GetPriority()` 返回 priority
- 实现 `GetName()` 返回 name
- 实现 `OnUpdate(float dt)`（可空实现）
- 公开成员变量用于测试验证：
  ```cpp
  int onAttachCount = 0;
  int onDetachCount = 0;
  int onUpdateCount = 0;
  ```
- 重写 `OnAttach(Scene*)` 时自增 `onAttachCount`
- 重写 `OnDetach()` 时自增 `onDetachCount`
- 重写 `OnUpdate(float)` 时自增 `onUpdateCount`

**依赖：** `include/Scene/ICapabilities/ICapabilities.h`（已存在）

---

## 文件 4：`CMakeLists.txt`（根目录，修改现有文件）

**修改内容：**
在 `add_library(NovaEngine_Core STATIC ...)` 的源文件列表中添加：
```cmake
"src/Scene/Capabilities/Render.cpp"
```

**依赖：** 文件 2 的路径确定

---

## 文件 5：`src/NovaEngine main.cpp`（修改现有文件）

**状态：** 当前包含大量硬编码渲染逻辑

**修改内容：**

### [1] 删除的代码
- 第68-97行：硬编码着色器源码
- 第76-117行：着色器编译和链接
- 第124-151行：硬编码 VAO/VBO/EBO 创建
- 第158-160行：`glGetUniformLocation`
- 第195行：`glClearColor` + `glClear`
- 第199-204行：`glUseProgram` + `glUniform` + `glDrawElements`

### [2] 新增 include
```cpp
#include "Scene/Core/Scene.h"
#include "Scene/Capabilities/Render.h"
#include "Scene/DataComponents/Mesh.h"
#include "Scene/DataComponents/Lighting/PointLight.h"
```

### [3] 新增逻辑（在 ImGui 初始化之后、主循环之前）
```cpp
SceneManager::Scene scene;
auto* render = new SceneManager::Capability::Render();
scene.AddCapability(render);
// 创建演示对象（例如 3x3 网格，带不同颜色）
// 每个对象：scene.CreateObject("Quad_N") → AddComponent<Mesh>
// 部分对象：AddComponent<PointLight>
scene.SceneBegin();
```

### [4] 主循环修改
- 删除 `glClear` / `glUseProgram` / `glDrawElements`
- 在 `ImGui::NewFrame()` 之后、`ImGui::Render()` 之前插入：`scene.Update(0.016f);`
- ImGui 渲染逻辑保持不变

### [5] 清理代码修改
- 删除 `glDeleteShader` / `glDeleteProgram`（由 `Render::OnDetach` 处理）
- 在 `glfwDestroyWindow` 之前插入：`scene.SceneEnd();`

**依赖：** 文件 1、2、4 完成

---

## 文件 6：`test/main_test.cpp`（修改现有文件）

**状态：** 当前只有一行 `#include "TestFramework.h"`

**实现内容：**

### [1] 第一阶段：单元测试（5组）
- **GameObject 生命周期**：创建、查询、销毁、ID 不复用
- **Component CRUD**：添加、查询、删除、重复添加、类型列表
- **Capability 生命周期**：注册、排序、OnAttach/OnUpdate/OnDetach
- **ForEach 遍历**：不同组件组合的批量查询
- **Command 队列**：PushCommand、FlushCommand、Update 中自动消费

### [2] 第二阶段：集成测试（1个完整用例）
- Scene 完整生命周期流程
- 10个对象 + 多种组件 + 3个 Capability + Command 队列
- 60帧 Update 循环模拟
- 部分清理 + 场景销毁

### [3] 第三阶段：压力测试（3个用例）
- **万家灯火**：1,000/5,000/10,000 个 Mesh+PointLight 对象
- **星海交辉**：50,000 对象，90%纯Mesh + 10%Mesh+PointLight
- **烽火连天**：5,000 静态 + 每帧500动态创建/销毁

**依赖：** 文件 3（MockCapability）、文件 5（main.cpp 渲染管线可用）

---

## 文件 7：`test/CMakeLists.txt`（视情况修改）

如果测试全部写在 `main_test.cpp` 中，无需修改。
如果拆分为多个测试文件，在此添加对应的 `.cpp` 源文件。

**依赖：** 文件 6 的测试文件结构确定

---

## 串行依赖链

```
文件1 → 文件2 → 文件4 → 文件5 → 文件6 → 文件7(可选)
                          ↗
文件3 ───────────────────┘
```

> 文件3 可与文件1/2/4 并行推进。

---

## 注意事项

### 1. 测试方案中的数值错误（写测试代码时注意修正）
- 集成测试 Step 7：OnDetach 预期 2 次 → **应为 3 次**
- 集成测试 Step 6：Mesh 数量预期 5 → **应为 4**
- 全文：组件名 "Light" → **统一为 "PointLight"**
- 单元测试 Capability Step 4：SceneEnd 后**不要调用** RemoveCapability

### 2. 已知限制
- 销毁 GameObject 时不会自动释放 GPU 资源（VAO/VBO/EBO）
- 压力测试中频繁创建/销毁对象会累积 GPU 内存泄漏
- 需要后续添加 `OnComponentRemoved` 钩子机制解决

### 3. 当前没有相机/投影矩阵
- 所有坐标在 NDC 空间 `[-1, 1]`
- 10,000 个对象排列成 100x100 网格，每个约 0.02 单位宽
- 对压力测试的网格排列来说足够