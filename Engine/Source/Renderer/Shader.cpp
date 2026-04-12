#include "Shader.h"

namespace Doppio::Render
{
	Shader::Shader( const std::string& FilePath )
	{
	}

	Shader::~Shader()
	{
	}

	Shader::Shader( Shader&& Other ) noexcept
	{
	}

	Shader& Shader::operator=( Shader&& Other ) noexcept
	{
		if ( this != &Other )
		{
		}
		
		return *this;
	}
}
