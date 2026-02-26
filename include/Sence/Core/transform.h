#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

namespace SenceManger
{
	class Sence;

	class Transform
	{
	private:

		glm::vec3 position;//位置
		glm::quat rotation;//旋转
		glm::vec3 scale;//缩放


	public:

		//构造函数
		Transform();
		//构造函数
		Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
		//析构函数
		~Transform() = default;


		//获取位置
		const glm::vec3  GetPosition() const;
		//设置位置
		void SetPosition(const glm::vec3& position);

		//获取旋转
		const glm::quat GetRotation() const;
		//设置旋转
		void SetRotation(const glm::quat& rotation);

		//获取缩放
		const glm::vec3 GetScale() const;
		//设置缩放
		void SetScale(const glm::vec3& scale);

		//获取变换矩阵
		glm::mat4 GetMatrix() const;

		//获取世界变换矩阵


		//计算变换矩阵


	};

}

