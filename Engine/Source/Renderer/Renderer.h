#pragma once

namespace Doppio::Render
{
	class Shader;
	class IndexBuffer;
	class VertexArray;

	class Renderer
	{
	public:
		Renderer() = delete;

		static void Init();
		static void Draw(const VertexArray& VertexArray, const IndexBuffer& IndexBuffer, const Shader& Shader);
		static void Clear();
	};
}
