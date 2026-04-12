#pragma once
#include <string>
#include <unordered_map>

#include "glm/vec4.hpp"

namespace Doppio::Render
{
	namespace Internal
	{
		struct ShaderProgramSource
		{
			std::string VertexSource;
			std::string FragmentSource;
		};
	}

	class Shader
	{
	public:
		Shader() = delete;
		explicit Shader( const std::string& FilePath );
		~Shader();

		Shader( const Shader& Other ) = delete;
		Shader& operator=( const Shader& Other ) = delete;

		Shader( Shader&& Other ) noexcept;
		Shader& operator=( Shader&& Other ) noexcept;

		void Bind() const;
		void Unbind() const;

		void SetUniform( const std::string& Name, const glm::vec4& Value );
		void SetUniform( const std::string& Name, int32_t Value );

	private:
		int32_t GetUniformLocation( const std::string& Name );

		Internal::ShaderProgramSource ParseShader();
		uint32_t Compile( uint32_t ShaderType, const std::string& ShaderCode );
		uint32_t Create( const std::string& VertexShader, const std::string& FragmentShader );
		
		void ValidateBindingState(const std::string& UniformName) const;

		uint32_t RendererId = 0;
		std::string FilePath = std::string();
		std::unordered_map<std::string, int32_t> UniformLocationCache;
	};
}
