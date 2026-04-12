#pragma once
#include <string>

namespace Doppio::Render
{
	class Shader
	{
	public:
		Shader() = delete;
		Shader(const std::string& FilePath);
		~Shader();
		
		Shader( const Shader& Other ) = delete;
		Shader& operator=( const Shader& Other ) = delete;

		Shader( Shader&& Other ) noexcept;
		Shader& operator=( Shader&& Other ) noexcept;
	};
}
