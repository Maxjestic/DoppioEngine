#pragma once
#include <cstdint>

namespace Doppio::Render
{
	class VertexBuffer;
	class VertexBufferLayout;

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray( const VertexArray& Other ) = delete;
		VertexArray& operator=( const VertexArray& Other ) = delete;

		VertexArray( VertexArray&& Other ) noexcept;
		VertexArray& operator=( VertexArray&& Other ) noexcept;

		void AddBuffer( const VertexBuffer& Buffer, const VertexBufferLayout& Layout ) const;
		void Bind() const;
		static void Unbind();

	private:
		uint32_t RendererId = 0;
	};
}
