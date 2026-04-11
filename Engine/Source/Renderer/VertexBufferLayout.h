#pragma once

#include <vector>

namespace Doppio::Render
{
	struct VertexBufferElement
	{
		uint32_t Type;
		uint32_t Count;
		uint8_t Normalized;

		static uint32_t GetSizeOfType( uint32_t Type );
		[[nodiscard]] uint32_t GetSizeOfType() const;
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() = default;
		~VertexBufferLayout() = default;

		VertexBufferLayout( const VertexBufferLayout& Other ) = delete;
		VertexBufferLayout& operator=( const VertexBufferLayout& Other ) = delete;

		VertexBufferLayout( VertexBufferLayout&& Other ) noexcept;
		VertexBufferLayout& operator=( VertexBufferLayout&& Other ) noexcept;

		template <typename T>
		void Push( uint32_t InComponentCount ) = delete;

		[[nodiscard]] const std::vector<VertexBufferElement>& GetElements() const;
		[[nodiscard]] uint32_t GetStride() const;

	private:
		std::vector<VertexBufferElement> Elements;
		uint32_t Stride = 0;
	};

	template <>
	void VertexBufferLayout::Push<float>( uint32_t InComponentCount );

	template <>
	void VertexBufferLayout::Push<uint32_t>( uint32_t InComponentCount );

	template <>
	void VertexBufferLayout::Push<uint8_t>( uint32_t InComponentCount );
}
