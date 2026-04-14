#pragma once
#include <string>

#include "glm/fwd.hpp"

namespace Doppio::Render
{
	class Texture
	{
	public:
		explicit Texture(const std::string& InFilePath);
		~Texture();

		Texture( const Texture& Other ) = delete;
		Texture& operator=( const Texture& Other ) = delete;

		Texture( Texture&& Other ) noexcept;
		Texture& operator=( Texture&& Other ) noexcept;
		
		void Bind(uint32_t Slot = 0) const;
		void Unbind() const;

		[[nodiscard]] int32_t GetWidth() const;
		[[nodiscard]] int32_t GetHeight() const;
		[[nodiscard]] int32_t GetBitsPerPixel() const;

	private:
		uint32_t RendererId = 0;
		std::string FilePath;
		uint8_t* LocalBuffer = nullptr;
		int32_t Width = 0;
		int32_t Height = 0;
		int32_t BitsPerPixel = 0;
	};
}
