#pragma once

namespace Rendering
{
	class Mesh
	{
	private:
		unsigned int VAO = 0; VBO = 0; EBO = 0;
		size_t IndexCount = 0;

		void SetupMesh();//创建VAO,VBO,EBO


	public:
		//构造函数
		Mesh();
		//析构函数
		~Mesh();

		//设置顶点数据
		void SetVertics

	};
}
