#include "Renderer.h"

#include <cstdio>
#include <print>
#include <glad/glad.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "GLFW/glfw3.h"

namespace
{
	constexpr auto ColorRed = "\x1b[31m";
	constexpr auto ColorYellow = "\x1b[33m";
	constexpr auto ColorMagenta = "\x1b[35m";
	constexpr auto ColorReset = "\x1b[0m";

	void GLDebugMessageCallback(GLenum InSource, GLenum InType, GLuint Id, GLenum InSeverity, GLsizei Length,
	                            const GLchar* Message, const void* Data)
	{
		const char* source;
		const char* type;
		const char* currentColor;
		FILE* outputStream;

		switch (InSource)
		{
		case GL_DEBUG_SOURCE_API:
			source = "API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			source = "WINDOW SYSTEM";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			source = "SHADER COMPILER";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			source = "THIRD PARTY";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			source = "APPLICATION";
			break;

		default:
		case GL_DEBUG_SOURCE_OTHER:
			source = "UNKNOWN";
			break;
		}

		switch (InType)
		{
		case GL_DEBUG_TYPE_ERROR:
			type = "ERROR";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			type = "DEPRECATED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			type = "UNDEFINED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
			type = "PORTABILITY";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:
			type = "PERFORMANCE";
			break;

		case GL_DEBUG_TYPE_OTHER:
			type = "OTHER";
			break;

		case GL_DEBUG_TYPE_MARKER:
			type = "MARKER";
			break;

		default:
			type = "UNKNOWN";
			break;
		}

		switch (InSeverity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			currentColor = ColorRed;
			outputStream = stderr;
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			currentColor = ColorRed;
			outputStream = stderr;
			break;

		case GL_DEBUG_SEVERITY_LOW:
			currentColor = ColorYellow;
			outputStream = stderr;
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			currentColor = ColorReset;
			outputStream = stdout;
			break;

		default:
			currentColor = ColorMagenta;
			outputStream = stderr;
			break;
		}

		std::println(outputStream,
		             "{}{}: {}, raised from {}: {}{}",
		             currentColor,
		             Id,
		             type,
		             source,
		             Message,
		             ColorReset);
	}
}

namespace Doppio::Render
{
	void Renderer::Init()
	{
		const int version = gladLoadGLLoader([](const char* Name) -> void*
		{
			return reinterpret_cast<void*>(glfwGetProcAddress(Name));
		});

		if (version == 0)
		{
			std::println(stderr, "Failed to initialize GLAD");
			return;
		}

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugMessageCallback, nullptr);
	}

	void Renderer::Draw(const VertexArray& VertexArray, const IndexBuffer& IndexBuffer, const Shader& Shader)
	{
		Shader.Bind();
		VertexArray.Bind();
		IndexBuffer.Bind();

		glDrawElements(GL_TRIANGLES, static_cast<int32_t>(IndexBuffer.GetCount()), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
