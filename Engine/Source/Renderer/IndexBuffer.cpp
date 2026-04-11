#include "IndexBuffer.h"

#include <glad/glad.h>

namespace Doppio::Render
{
	IndexBuffer::IndexBuffer( const uint32_t InCount, const uint32_t* InData )
		: Count( InCount )
	{
		glGenBuffers( 1, &RendererId );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, RendererId );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, InCount * sizeof( uint32_t ), InData, GL_STATIC_DRAW );
	}

	IndexBuffer::IndexBuffer( IndexBuffer&& Other ) noexcept
		: RendererId( Other.RendererId )
	{
		Other.RendererId = 0;
	}

	IndexBuffer& IndexBuffer::operator=( IndexBuffer&& Other ) noexcept
	{
		if ( this == &Other )
		{
			return *this;
		}

		RendererId = Other.RendererId;
		Other.RendererId = 0;
		return *this;
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers( 1, &RendererId );
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, RendererId );
	}

	void IndexBuffer::Unbind()
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}

	uint32_t IndexBuffer::GetCount() const
	{
		return Count;
	}
}
