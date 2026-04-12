#pragma once
#include <cstdint>

namespace Doppio::Render
{
	class VertexBuffer
	{
	public:
		VertexBuffer() = delete;
		VertexBuffer( uint32_t InSize, const void* InData );
		~VertexBuffer();

		VertexBuffer( const VertexBuffer& Other ) = delete;
		VertexBuffer& operator=( const VertexBuffer& Other ) = delete;

		VertexBuffer( VertexBuffer&& Other ) noexcept;
		VertexBuffer& operator=( VertexBuffer&& Other ) noexcept;

		void Bind() const;
		void Unbind() const;

	private:
		/// Internal Renderer Id
		uint32_t RendererId = 0;
	};
}
