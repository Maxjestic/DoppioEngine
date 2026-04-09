#include "Renderer.h"

#include <cstdio>
#include <print>
#include <glad/glad.h>

#include "GLFW/glfw3.h"

namespace
{
constexpr const char* ColorRed = "\x1b[31m";
constexpr const char* ColorYellow = "\x1b[33m";
constexpr const char* ColorMagenta = "\x1b[35m";
constexpr const char* ColorReset = "\x1b[0m";

void GLDebugMessageCallback( GLenum InSource, GLenum InType, GLuint Id, GLenum InSeverity, GLsizei Length,
                             const GLchar* Message, const void* Data )
{
	const char* Source;
	const char* Type;
	const char* CurrentColor;
	FILE* OutputStream;

	switch ( InSource )
	{
	case GL_DEBUG_SOURCE_API:
		Source = "API";
		break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		Source = "WINDOW SYSTEM";
		break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		Source = "SHADER COMPILER";
		break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		Source = "THIRD PARTY";
		break;

	case GL_DEBUG_SOURCE_APPLICATION:
		Source = "APPLICATION";
		break;

	default:
	case GL_DEBUG_SOURCE_OTHER:
		Source = "UNKNOWN";
		break;
	}

	switch ( InType )
	{
	case GL_DEBUG_TYPE_ERROR:
		Type = "ERROR";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		Type = "DEPRECATED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		Type = "UNDEFINED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		Type = "PORTABILITY";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		Type = "PERFORMANCE";
		break;

	case GL_DEBUG_TYPE_OTHER:
		Type = "OTHER";
		break;

	case GL_DEBUG_TYPE_MARKER:
		Type = "MARKER";
		break;

	default:
		Type = "UNKNOWN";
		break;
	}

	switch ( InSeverity )
	{
	case GL_DEBUG_SEVERITY_HIGH:
		CurrentColor = ColorRed;
		OutputStream = stderr;
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		CurrentColor = ColorRed;
		OutputStream = stderr;
		break;

	case GL_DEBUG_SEVERITY_LOW:
		CurrentColor = ColorYellow;
		OutputStream = stderr;
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		CurrentColor = ColorReset;
		OutputStream = stdout;
		break;

	default:
		CurrentColor = ColorMagenta;
		OutputStream = stderr;
		break;
	}

	std::println( OutputStream,
	              "{}{}: {}, raised from {}: {}{}",
	              CurrentColor,
	              Id,
	              Type,
	              Source,
	              Message,
	              ColorReset );
}
}

namespace Doppio::Render
{
void Renderer::Init()
{
	const int Version = gladLoadGLLoader( []( const char* Name ) -> void*
	{
		return reinterpret_cast<void*>(glfwGetProcAddress( Name ));
	} );

	if ( Version == 0 )
	{
		std::println( stderr, "Failed to initialize GLAD" );
		return;
	}

	glEnable( GL_DEBUG_OUTPUT );
	glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
	glDebugMessageCallback( GLDebugMessageCallback, nullptr );
}
}
