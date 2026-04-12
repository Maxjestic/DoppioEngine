#include "VertexArray.h"

#include <glad/glad.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Doppio::Render
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays( 1, &RendererId );
		glBindVertexArray( RendererId );
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays( 1, &RendererId );
	}

	VertexArray::VertexArray( VertexArray&& Other ) noexcept
		: RendererId( Other.RendererId )
	{
		Other.RendererId = 0;
	}

	VertexArray& VertexArray::operator=( VertexArray&& Other ) noexcept
	{
		if ( this != &Other )
		{
			glDeleteVertexArrays( 1, &RendererId );
			
			RendererId = Other.RendererId;
			
			Other.RendererId = 0;
		}

		return *this;
	}

	void VertexArray::AddBuffer( const VertexBuffer& Buffer, const VertexBufferLayout& Layout ) const
	{
		Bind();
		Buffer.Bind();
		const auto& elements = Layout.GetElements();
		uint32_t offset = 0;
		for ( size_t i = 0; i < elements.size(); ++i )
		{
			const auto& element = elements[i];
			glEnableVertexAttribArray( i );
			glVertexAttribPointer( i,
			                       element.ComponentCount,
			                       element.Type,
			                       element.Normalized,
			                       Layout.GetStride(),
			                       reinterpret_cast<const void*>(offset) ); // NOLINT(performance-no-int-to-ptr)
			offset += element.ComponentCount * element.GetSizeOfType();
		}
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray( RendererId );
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray( 0 );
	}
}
