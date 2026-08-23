#include "Shader.h"

#include "ThirdParty/GladWrapper.h"
#include "ThirdParty/GLMWrapper.h"

#include "Core/Assert.h"

namespace Doppio::Render
{
	Shader::Shader(const std::string& FilePath)
		: FilePath(FilePath)
	{
		const auto [VertexSource, FragmentSource] = ParseShader();
		RendererId = Create(VertexSource, FragmentSource);
	}

	Shader::~Shader()
	{
		glDeleteProgram(RendererId);
	}

	Shader::Shader(Shader&& Other) noexcept
		: RendererId(Other.RendererId), FilePath(Other.FilePath),
		  UniformLocationCache(std::move(Other.UniformLocationCache))
	{
		Other.RendererId = 0;
		Other.FilePath = "";
		Other.UniformLocationCache = {};
	}

	Shader& Shader::operator=(Shader&& Other) noexcept
	{
		if (this != &Other)
		{
			glDeleteProgram(RendererId);

			RendererId = Other.RendererId;
			FilePath = Other.FilePath;
			UniformLocationCache = std::move(Other.UniformLocationCache);

			Other.RendererId = 0;
			Other.FilePath = "";
			Other.UniformLocationCache = {};
		}

		return *this;
	}

	void Shader::Bind() const
	{
		glUseProgram(RendererId);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniform(const std::string& Name, const glm::vec4& Value)
	{
		ValidateBindingState(Name);
		glUniform4f(GetUniformLocation(Name), Value.x, Value.y, Value.z, Value.w);
	}

	void Shader::SetUniform(const std::string& Name, const int32_t Value)
	{
		ValidateBindingState(Name);
		glUniform1i(GetUniformLocation(Name), Value);
	}

	void Shader::SetUniform(const std::string& Name, const glm::mat4& Value)
	{
		ValidateBindingState(Name);
		glUniformMatrix4fv(GetUniformLocation(Name), 1, GL_FALSE, &Value[0][0]);
	}

	int32_t Shader::GetUniformLocation(const std::string& Name)
	{
		const auto iterator = UniformLocationCache.find(Name);
		if (iterator != UniformLocationCache.end())
		{
			return iterator->second;
		}

		const int32_t location = glGetUniformLocation(RendererId, Name.c_str());
		if (location == -1)
		{
			std::println(stderr, "Warning: Could not find uniform: {}", Name);
			return location;
		}

		UniformLocationCache[Name] = location;
		return location;
	}

	Internal::ShaderProgramSource Shader::ParseShader()
	{
		if (FilePath.empty())
		{
			return {};
		}

		std::ifstream file(FilePath);

		if (!file.is_open())
		{
			std::println(stderr, "ERROR: Could not open shader file at: {}", FilePath);
			return {"", ""};
		}

		enum class ShaderType : std::int8_t
		{
			None = -1,
			Vertex = 0,
			Fragment = 1
		};

		std::string line;
		std::stringstream stringStream[2];
		auto shaderType = ShaderType::None;
		while (getline(file, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					shaderType = ShaderType::Vertex;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					shaderType = ShaderType::Fragment;
				}
			}
			else if (shaderType != ShaderType::None)
			{
				stringStream[static_cast<int32_t>(shaderType)] << line << '\n';
			}
		}

		Internal::ShaderProgramSource shader;
		shader.VertexSource = stringStream[static_cast<int32_t>(ShaderType::Vertex)].str();
		shader.FragmentSource = stringStream[static_cast<int32_t>(ShaderType::Fragment)].str();

		return shader;
	}

	uint32_t Shader::Compile(const uint32_t ShaderType, const std::string& ShaderCode)
	{
		const uint32_t vertexShaderId = glCreateShader(ShaderType);
		const char* sourcePointer = ShaderCode.c_str();
		glShaderSource(vertexShaderId, 1, &sourcePointer, nullptr);
		glCompileShader(vertexShaderId);

		int32_t result;
		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int32_t length;
			glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &length);

			std::vector<char> infoLog(length);
			glGetShaderInfoLog(vertexShaderId, length, &length, infoLog.data());

			std::println(stderr,
			             "Failed to compile {} shader: {}",
			             (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment"),
			             infoLog.data());

			glDeleteShader(vertexShaderId);
			return 0;
		}

		return vertexShaderId;
	}

	uint32_t Shader::Create(const std::string& VertexShader, const std::string& FragmentShader)
	{
		const uint32_t programId = glCreateProgram();
		const uint32_t vertexShaderId = Compile(GL_VERTEX_SHADER, VertexShader);
		const uint32_t fragmentShaderId = Compile(GL_FRAGMENT_SHADER, FragmentShader);

		glAttachShader(programId, vertexShaderId);
		glAttachShader(programId, fragmentShaderId);
		glLinkProgram(programId);
		glValidateProgram(programId);

		int32_t result;
		glGetProgramiv(programId, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int32_t length;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);

			std::vector<char> infoLog(length);
			glGetProgramInfoLog(programId, length, &length, infoLog.data());

			std::println(stderr, "Failed to link shader program: {}", infoLog.data());
			glDeleteProgram(programId);
			glDeleteShader(vertexShaderId);
			glDeleteShader(fragmentShaderId);
			return 0;
		}

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return programId;
	}

	void Shader::ValidateBindingState(const std::string& UniformName) const
	{
#ifdef DOPPIO_DEBUG
		int32_t currentProgram = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		const uint32_t uCurrentProgram = static_cast<uint32_t>(currentProgram);

		DE_CORE_ASSERT(uCurrentProgram == RendererId,
		               "Attempted to set uniform '{}' without binding the Shader first!",
		               UniformName);
#endif
	}
}
