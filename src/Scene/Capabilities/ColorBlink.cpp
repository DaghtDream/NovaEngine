#include "Capabilities/ColorBlink.h"


namespace SceneManager::Capability
{
	void ColorBlink::OnAttach(Scene* scene)
	{
		m_scene = scene;
	}

	void ColorBlink::OnUpdate(float dt)
	{
		m_time += dt;

		// 高速遍历所有挂载了 PointLight 的实体
		m_scene->ForEach<PointLight>([this](uint32_t id, PointLight& light) {

			// 纯粹修改 RGB 值，映射到 [0.0, 1.0] 之间
			// 使用不同的乘数制造出一种“波浪式”的 RGB 霓虹交替效果
			light.color.r = (std::sin(m_time * 3.0f + id * 0.1f) + 1.0f) * 0.5f;
			light.color.g = (std::sin(m_time * 2.0f + id * 0.2f) + 1.0f) * 0.5f;
			light.color.b = (std::sin(m_time * 4.0f + id * 0.05f) + 1.0f) * 0.5f;
			});
	}

	void ColorBlink::OnDetach() {}
}