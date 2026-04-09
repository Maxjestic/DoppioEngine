#pragma once

namespace Doppio::Render
{
class IndexBuffer
{
public:
	IndexBuffer() = delete;
	IndexBuffer( unsigned int InCount, const unsigned int* InData );
	~IndexBuffer();
	
	IndexBuffer( const IndexBuffer& Other ) = delete;
	IndexBuffer& operator=( const IndexBuffer& Other ) = delete;
	
	IndexBuffer( IndexBuffer&& Other ) noexcept;
	IndexBuffer& operator=( IndexBuffer&& Other ) noexcept;

	void Bind() const;
	void Unbind() const;

	[[nodiscard]] unsigned int GetCount() const;

private:
	/// Internal Renderer Id
	unsigned int RendererId = 0;
	
	unsigned int Count = 0;
};
}
