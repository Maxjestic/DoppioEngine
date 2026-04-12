#include "VertexBufferLayout.h"

#include "glad/glad.h"

namespace Doppio::Render
{
	uint32_t VertexBufferElement::GetSizeOfType( const uint32_t Type )
	{
		switch ( Type )
		{
		case GL_FLOAT:
			return sizeof( float );
		case GL_UNSIGNED_INT:
			return sizeof( uint32_t );
		case GL_UNSIGNED_BYTE:
			return sizeof( uint8_t );
		default:
			_ASSERT( false );
			return 0;
		}
	}

	uint32_t VertexBufferElement::GetSizeOfType() const
	{
		return GetSizeOfType( Type );
	}

	template <>
	void VertexBufferLayout::Push<float>( const uint32_t InComponentCount )
	{
		Elements.push_back( { GL_FLOAT, InComponentCount, GL_FALSE } );
		Stride += InComponentCount * VertexBufferElement::GetSizeOfType( GL_FLOAT );
	}

	template <>
	void VertexBufferLayout::Push<uint32_t>( const uint32_t InComponentCount )
	{
		Elements.push_back( { GL_UNSIGNED_INT, InComponentCount, GL_FALSE } );
		Stride += InComponentCount * VertexBufferElement::GetSizeOfType( GL_UNSIGNED_INT );
	}

	template <>
	void VertexBufferLayout::Push<uint8_t>( const uint32_t InComponentCount )
	{
		Elements.push_back( { GL_UNSIGNED_BYTE, InComponentCount, GL_TRUE } );
		Stride += InComponentCount * VertexBufferElement::GetSizeOfType( GL_UNSIGNED_BYTE );
	}

	VertexBufferLayout::VertexBufferLayout( VertexBufferLayout&& Other ) noexcept
		: Elements( std::move( Other.Elements ) )
		  , Stride( Other.Stride )
	{
		Other.Stride = 0;
	}

	VertexBufferLayout& VertexBufferLayout::operator=( VertexBufferLayout&& Other ) noexcept
	{
		if ( this != &Other )
		{
			Elements = std::move( Other.Elements );
			Stride = Other.Stride;
			Other.Stride = 0;
		}

		return *this;
	}

	const std::vector<VertexBufferElement>& VertexBufferLayout::GetElements() const
	{
		return Elements;
	}

	uint32_t VertexBufferLayout::GetStride() const
	{
		return Stride;
	}
}
