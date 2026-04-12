#include <fstream>
#include <print>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBufferLayout.h"


namespace
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	ShaderProgramSource ParseShader( const std::string& Filepath )
	{
		std::ifstream file( Filepath );

		if ( !file.is_open() )
		{
			std::println( stderr, "ERROR: Could not open shader file at: {}", Filepath );
			return { "", "" };
		}

		enum class ShaderType : int8_t
		{
			None = -1,
			Vertex = 0,
			Fragment = 1
		};

		std::string line;
		std::stringstream stringStream[2];
		ShaderType shaderType = ShaderType::None;
		while ( getline( file, line ) )
		{
			if ( line.find( "#shader" ) != std::string::npos )
			{
				if ( line.find( "vertex" ) != std::string::npos )
				{
					shaderType = ShaderType::Vertex;
				}
				else if ( line.find( "fragment" ) != std::string::npos )
				{
					shaderType = ShaderType::Fragment;
				}
			}
			else
			{
				stringStream[static_cast<int>(shaderType)] << line << '\n';
			}
		}

		ShaderProgramSource shader;
		shader.VertexSource = stringStream[static_cast<int>(ShaderType::Vertex)].str();
		shader.FragmentSource = stringStream[static_cast<int>(ShaderType::Fragment)].str();

		return shader;
	}

	unsigned int CompileShader( const unsigned int ShaderType, const std::string& ShaderCode )
	{
		const unsigned int vertexShaderId = glCreateShader( ShaderType );
		const char* sourcePointer = ShaderCode.c_str();
		glShaderSource( vertexShaderId, 1, &sourcePointer, nullptr );
		glCompileShader( vertexShaderId );

		int result;
		glGetShaderiv( vertexShaderId, GL_COMPILE_STATUS, &result );
		if ( result == GL_FALSE )
		{
			int length;
			glGetShaderiv( vertexShaderId, GL_INFO_LOG_LENGTH, &length );

			auto infoLog = static_cast<char*>(alloca( length * sizeof( char ) ));
			glGetShaderInfoLog( vertexShaderId, length, &length, infoLog );

			std::println( stderr,
			              "Failed to compile {} shader: {}",
			              ( ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment" ),
			              infoLog );

			glDeleteShader( vertexShaderId );
			return 0;
		}

		return vertexShaderId;
	}

	unsigned int CreateShader( const std::string& VertexShader, const std::string& FragmentShader )
	{
		const unsigned int programId = glCreateProgram();
		const unsigned int vertexShaderId = CompileShader( GL_VERTEX_SHADER, VertexShader );
		const unsigned int fragmentShaderId = CompileShader( GL_FRAGMENT_SHADER, FragmentShader );

		glAttachShader( programId, vertexShaderId );
		glAttachShader( programId, fragmentShaderId );
		glLinkProgram( programId );
		glValidateProgram( programId );

		int result;
		glGetProgramiv( programId, GL_LINK_STATUS, &result );
		if ( result == GL_FALSE )
		{
			int length;
			glGetProgramiv( programId, GL_INFO_LOG_LENGTH, &length );
			auto infoLog = static_cast<char*>(alloca( length * sizeof( char ) ));
			glGetProgramInfoLog( programId, length, &length, infoLog );
			std::println( stderr, "Failed to link shader program: {}", infoLog );
			glDeleteProgram( programId );
			glDeleteShader( vertexShaderId );
			glDeleteShader( fragmentShaderId );
			return 0;
		}

		glDeleteShader( vertexShaderId );
		glDeleteShader( fragmentShaderId );

		return programId;
	}
}

int main()
{
	if ( !glfwInit() )
	{
		std::println( stderr, "Failed to initialize GLFW" );
		return -1;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );

	GLFWwindow* window = glfwCreateWindow( 1280, 720, "DoppioEngine", nullptr, nullptr );
	if ( window == nullptr )
	{
		std::println( stderr, "Failed to create GLFW window" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );

	glfwSwapInterval( 1 );

	const int version = gladLoadGLLoader( []( const char* Name ) -> void*
	{
		return reinterpret_cast<void*>(glfwGetProcAddress( Name ));
	} );

	if ( version == 0 )
	{
		std::println( stderr, "Failed to initialize GLAD" );
		return -1;
	}

	Doppio::Render::Renderer::Init();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& inOut = ImGui::GetIO();

	inOut.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	inOut.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	inOut.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if ( inOut.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
	{
		style.WindowRounding = 2.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL( window, true );
	ImGui_ImplOpenGL3_Init( "#version 460 core" );

	constexpr float positions[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f,
	};

	constexpr unsigned int indices[]
	{
		0, 1, 2,
		2, 3, 0
	};

	const Doppio::Render::VertexArray vertexArray;
	const Doppio::Render::VertexBuffer vertexBuffer{ 4 * 2 * sizeof( float ), positions };

	Doppio::Render::VertexBufferLayout layout;
	layout.Push<float>( 2 );

	vertexArray.AddBuffer( vertexBuffer, layout );

	const Doppio::Render::IndexBuffer indexBuffer{ 6, indices };

	const ShaderProgramSource shaderSource = ParseShader( "Engine/Resources/Shaders/Basic.shader" );

	const unsigned int shader = CreateShader( shaderSource.VertexSource, shaderSource.FragmentSource );

	vertexArray.Unbind();
	vertexBuffer.Unbind();
	indexBuffer.Unbind();
	glUseProgram( 0 );

	float red = 0.0f;
	float increment = 0.01f;

	while ( !glfwWindowShouldClose( window ) )
	{
		glClear( GL_COLOR_BUFFER_BIT );

		glUseProgram( shader );
		glUniform4f( 1, red, 0.3f, 0.8f, 1.0f );

		vertexArray.Bind();
		indexBuffer.Bind();

		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );

		if ( red > 1.0f )
		{
			increment = -0.01f;
		}
		else if ( red < 0.0f )
		{
			increment = 0.01f;
		}

		red += increment;

		glfwSwapBuffers( window );

		glfwPollEvents();
	}

	glDeleteProgram( shader );
	glfwDestroyWindow( window );
	glfwTerminate();

	return 0;
}
