#pragma once

#include <cstdint>

namespace Doppio::Render
{
	class IndexBuffer
	{
	public:
		IndexBuffer() = delete;
		IndexBuffer( uint32_t InCount, const uint32_t* InData );
		~IndexBuffer();

		IndexBuffer( const IndexBuffer& Other ) = delete;
		IndexBuffer& operator=( const IndexBuffer& Other ) = delete;

		IndexBuffer( IndexBuffer&& Other ) noexcept;
		IndexBuffer& operator=( IndexBuffer&& Other ) noexcept;

		void Bind() const;
		static void Unbind();

		[[nodiscard]] uint32_t GetCount() const;

	private:
		/// Internal Renderer Id
		uint32_t RendererId = 0;

		uint32_t Count = 0;
	};
}
