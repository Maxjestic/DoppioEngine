#include "IndexBuffer.h"

#include <glad/glad.h>

namespace Doppio::Render
{
IndexBuffer::IndexBuffer( const unsigned int InCount, const unsigned int* InData )
	: Count( InCount )
{
	glGenBuffers( 1, &RendererId );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, RendererId );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, InCount * sizeof( unsigned int ), InData, GL_STATIC_DRAW );
}

IndexBuffer::IndexBuffer( IndexBuffer&& Other ) noexcept
	: RendererId( Other.RendererId )
{
}

IndexBuffer& IndexBuffer::operator=( IndexBuffer&& Other ) noexcept
{
	if ( this == &Other )
		return *this;
	RendererId = Other.RendererId;
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

void IndexBuffer::Unbind() const
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

unsigned int IndexBuffer::GetCount() const
{
	return Count;
}
}
