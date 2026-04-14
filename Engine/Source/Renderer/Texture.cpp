#include "Texture.h"

#include <glad/glad.h>

#include "ThirdParty/stb_image/stb_image.h"

namespace Doppio::Render
{
	Texture::Texture(const std::string& InFilePath)
		: FilePath(InFilePath)
	{
		stbi_set_flip_vertically_on_load( 1 );
		LocalBuffer = stbi_load( FilePath.c_str(), &Width, &Height, &BitsPerPixel, 4 );
		
		glGenTextures(1, &RendererId);
		glBindTexture(GL_TEXTURE_2D, RendererId);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		if ( LocalBuffer != nullptr )
		{
			stbi_image_free( LocalBuffer );
			LocalBuffer = nullptr;
		}
	}

	Texture::~Texture()
	{
		glDeleteTextures( 1, &RendererId );
	}

	Texture::Texture( Texture&& Other ) noexcept
		: RendererId( Other.RendererId ), FilePath( Other.FilePath ), LocalBuffer( Other.LocalBuffer ),
		  Width( Other.Width ), Height( Other.Height ), BitsPerPixel( Other.BitsPerPixel )
	{
		Other.RendererId = 0;
		Other.FilePath = "";
		Other.LocalBuffer = nullptr;
		Other.Width = 0;
		Other.Height = 0;
		Other.BitsPerPixel = 0;
	}

	Texture& Texture::operator=( Texture&& Other ) noexcept
	{
		if ( this != &Other )
		{
			RendererId = Other.RendererId;
			FilePath = Other.FilePath;
			LocalBuffer = Other.LocalBuffer;
			Width = Other.Width;
			Height = Other.Height;
			BitsPerPixel = Other.BitsPerPixel;

			Other.RendererId = 0;
			Other.FilePath = "";
			Other.LocalBuffer = nullptr;
			Other.Width = 0;
			Other.Height = 0;
			Other.BitsPerPixel = 0;
		}
		
		return *this;
	}

	void Texture::Bind( const uint32_t Slot /*= 0*/) const
	{
		glActiveTexture( GL_TEXTURE0 + Slot );
		glBindTexture( GL_TEXTURE_2D, RendererId );
	}

	void Texture::Unbind() const
	{
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	int32_t Texture::GetWidth() const
	{
		return Width;
	}

	int32_t Texture::GetHeight() const
	{
		return Height;
	}

	int32_t Texture::GetBitsPerPixel() const
	{
		return BitsPerPixel;
	}
}
