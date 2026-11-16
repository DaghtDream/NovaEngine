# GLSL uniform 显式 location（C 方案）学习笔记

> 知识拓展：本文仅为了解「C 方案」的原理与取舍。你的 M1 实际采用 **B 方案**（C++ 端缓存 location），C 方案先了解即可，不必实现。

---

## 一、uniform 与 location 是什么

在 GLSL 中，`uniform` 是一种"只读的全局变量"：CPU（C++ 端）每帧把数据传进 GPU，程序里所有顶点/片元共享同一份值。典型例子：

```glsl
uniform mat4 uModel;
uniform vec4 uColor;
```

CPU 要把数据写进某个 uniform，必须**先知道它在着色器程序里的"位置编号"（location）**。

---

## 二、默认机制：按名字查 location（B 方案依赖它）

默认情况下，你不手动指定 location，而是用函数按"名字"去查：

```cpp
GLint loc = glGetUniformLocation(program, "uModel");  // 按名字查
glUniformMatrix4fv(loc, 1, GL_FALSE, value);
```

`glGetUniformLocation` 本质是一次"字符串 → 编号"的哈希查找，有两个特点：

1. 每次调用都有开销（所以 **B 方案把结果缓存起来**：查一次、复用多次）；
2. 如果名字拼错、或该 uniform 被编译器优化掉了，会返回 `-1`（这其实是排错的好线索）。

---

## 三、C 方案：在着色器里"写死" location

GLSL 4.3+ 允许在**声明 uniform 时**，用 `layout(location = N)` 直接指定它的 location：

```glsl
#version 430 core

layout(location = 0) uniform mat4 uModel;
layout(location = 1) uniform vec4 uColor;

void main() { /* ... */ }
```

这样 `uModel` 的 location 被硬定为 `0`，`uColor` 被硬定为 `1`，**不再需要按名字查找**。

---

## 四、C 方案下，CPU 端怎么写

CPU 端直接用写死的数字，连 `glGetUniformLocation` 都不用调：

```cpp
glUniformMatrix4fv(0, 1, GL_FALSE, value);  // 直接写 location 0
glUniform4f(1, r, g, b, a);                  // 直接写 location 1
```

> 注意：实际工程里通常仍会用常量或枚举给 `0`、`1` 起个有意义的名字（如 `kUModelLocation`），避免"魔法数字"。

---

## 五、B vs C 取舍

| 维度 | B：C++ 端缓存 location | C：着色器显式指定 location |
|---|---|---|
| 查 location | 第一次按名字查，之后缓存 | 完全不用查，直接写数字 |
| 代码量 | C++ 端多一个 map 缓存 | 着色器端多 `layout(location=N)` |
| 名字依赖 | 依赖 uniform 名字字符串 | 不依赖名字，依赖固定编号 |
| 排错 | 名字拼错 → 返回 -1，容易发现 | 编号写错 → 传错数据，更隐蔽 |
| 灵活性 | 改 uniform 名不用改 C++ | 改编号要两端同步 |
| 多 program 场景 | 各自独立查表 | 可约定"同一套槽位约定"（见下） |

---

## 六、C 方案两个额外价值（你可能没想过）

1. **多 shader program 共享槽位约定**：当项目里有多个着色器（基础着色器、光照着色器、后处理……），C 方案可以统一约定"location 0 永远是 MVP、location 1 永远是颜色"，这样 CPU 端能写一套通用的传参逻辑，不关心具体是哪个 program。

2. **热重载更稳**：B 方案依赖 uniform 名字字符串，着色器热重载后如果某个 uniform 被改名/删除，字符串查找会静默失效；C 方案用固定编号，行为更可预期。

---

## 七、结论

- 你的 M1 用 **B**（常规、名字语义清晰、排错友好），完全正确。
- **C 方案先作为知识储备**，等你以后写"多个着色器 + 统一传参框架"或"着色器热重载"时，会自然理解它的价值。
