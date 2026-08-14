#include "VertexBuffer.h"

#include <glad/glad.h>

namespace Doppio::Render
{
	VertexBuffer::VertexBuffer(const uint32_t InSize, const void* InData)
	{
		glGenBuffers(1, &RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, RendererId);
		glBufferData(GL_ARRAY_BUFFER, InSize, InData, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& Other) noexcept
		: RendererId(Other.RendererId)
	{
		Other.RendererId = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& Other) noexcept
	{
		if (this != &Other)
		{
			RendererId = Other.RendererId;
			Other.RendererId = 0;
		}

		return *this;
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &RendererId);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, RendererId);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
