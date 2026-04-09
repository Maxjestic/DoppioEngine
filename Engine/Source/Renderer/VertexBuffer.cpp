#include "VertexBuffer.h"

#include <glad/glad.h>

namespace Doppio::Render
{
VertexBuffer::VertexBuffer( const unsigned int InSize, const void* Data )
{
	glGenBuffers( 1, &RendererId );
	glBindBuffer( GL_ARRAY_BUFFER, RendererId );
	glBufferData( GL_ARRAY_BUFFER, InSize, Data, GL_STATIC_DRAW );
}

VertexBuffer::VertexBuffer( VertexBuffer&& Other ) noexcept
	: RendererId( Other.RendererId )
{
}

VertexBuffer& VertexBuffer::operator=( VertexBuffer&& Other ) noexcept
{
	if ( this == &Other )
		return *this;
	RendererId = Other.RendererId;
	return *this;
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers( 1, &RendererId );
}

void VertexBuffer::Bind() const
{
	glBindBuffer( GL_ARRAY_BUFFER, RendererId );
}

void VertexBuffer::Unbind() const
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}
}
