#pragma once

namespace Doppio::Render
{
class VertexBuffer
{
public:
	VertexBuffer() = delete;
	VertexBuffer( unsigned int InSize, const void* InData );
	~VertexBuffer();
	
	VertexBuffer( const VertexBuffer& Other ) = delete;
	VertexBuffer& operator=( const VertexBuffer& Other ) = delete;
	
	VertexBuffer( VertexBuffer&& Other ) noexcept;
	VertexBuffer& operator=( VertexBuffer&& Other ) noexcept;

	void Bind() const;
	void Unbind() const;

private:
	/// Internal Renderer Id
	unsigned int RendererId = 0;
};
}
