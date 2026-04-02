#include <fstream>
#include <print>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	ShaderProgramSource ParseShader( const std::string& Filepath )
	{
		std::ifstream File( Filepath );

		if ( !File.is_open() )
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

		std::string Line;
		std::stringstream StringStream[2];
		ShaderType ShaderType = ShaderType::None;
		while ( getline( File, Line ) )
		{
			if ( Line.find( "#shader" ) != std::string::npos )
			{
				if ( Line.find( "vertex" ) != std::string::npos )
				{
					ShaderType = ShaderType::Vertex;
				}
				else if ( Line.find( "fragment" ) != std::string::npos )
				{
					ShaderType = ShaderType::Fragment;
				}
			}
			else
			{
				StringStream[static_cast<int>(ShaderType)] << Line << '\n';
			}
		}

		ShaderProgramSource Shader;
		Shader.VertexSource = StringStream[static_cast<int>(ShaderType::Vertex)].str();
		Shader.FragmentSource = StringStream[static_cast<int>(ShaderType::Fragment)].str();

		return Shader;
	}

	unsigned int CompileShader( const unsigned int ShaderType, const std::string& ShaderCode )
	{
		const unsigned int VertexShaderId = glCreateShader( ShaderType );
		const char* SourcePointer = ShaderCode.c_str();
		glShaderSource( VertexShaderId, 1, &SourcePointer, nullptr );
		glCompileShader( VertexShaderId );

		int Result;
		glGetShaderiv( VertexShaderId, GL_COMPILE_STATUS, &Result );
		if ( Result == GL_FALSE )
		{
			int Length;
			glGetShaderiv( VertexShaderId, GL_INFO_LOG_LENGTH, &Length );

			auto InfoLog = static_cast<char*>(alloca( Length * sizeof( char ) ));
			glGetShaderInfoLog( VertexShaderId, Length, &Length, InfoLog );

			std::println( stderr,
			              "Failed to compile {} shader: {}",
			              ( ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment" ),
			              InfoLog );

			glDeleteShader( VertexShaderId );
			return 0;
		}

		return VertexShaderId;
	}

	unsigned int CreateShader( const std::string& VertexShader, const std::string& FragmentShader )
	{
		const unsigned int ProgramId = glCreateProgram();
		const unsigned int VertexShaderId = CompileShader( GL_VERTEX_SHADER, VertexShader );
		const unsigned int FragmentShaderId = CompileShader( GL_FRAGMENT_SHADER, FragmentShader );

		glAttachShader( ProgramId, VertexShaderId );
		glAttachShader( ProgramId, FragmentShaderId );
		glLinkProgram( ProgramId );
		glValidateProgram( ProgramId );

		glDeleteShader( VertexShaderId );
		glDeleteShader( FragmentShaderId );

		return ProgramId;
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
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	GLFWwindow* Window = glfwCreateWindow( 1280, 720, "DoppioEngine", nullptr, nullptr );
	if ( Window == nullptr )
	{
		std::println( stderr, "Failed to create GLFW window" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( Window );

	const int Version = gladLoadGLLoader( []( const char* Name ) -> void*
	{
		return reinterpret_cast<void*>(glfwGetProcAddress( Name ));
	} );

	if ( Version == 0 )
	{
		std::println( stderr, "Failed to initialize GLAD" );
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& InOut = ImGui::GetIO();

	InOut.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	InOut.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	InOut.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& Style = ImGui::GetStyle();
	if ( InOut.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
	{
		Style.WindowRounding = 2.0f;
		Style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL( Window, true );
	ImGui_ImplOpenGL3_Init( "#version 430 core" );

	constexpr float Positions[6] = {
		-0.5, -0.5,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	unsigned int VertexArray;
	glGenVertexArrays( 1, &VertexArray );
	glBindVertexArray( VertexArray );

	unsigned int VertexBuffer;
	glGenBuffers( 1, &VertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, 6 * sizeof( float ), Positions, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 2, nullptr );

	const ShaderProgramSource ShaderSource = ParseShader( "Resources/Shaders/Basic.shader" );

	const unsigned int Shader = CreateShader( ShaderSource.VertexSource, ShaderSource.FragmentSource );
	glUseProgram( Shader );

	while ( !glfwWindowShouldClose( Window ) )
	{
		glClear( GL_COLOR_BUFFER_BIT );
		glDrawArrays( GL_TRIANGLES, 0, 3 );

		glfwSwapBuffers( Window );

		glfwPollEvents();
	}

	glDeleteProgram( Shader );
	glfwDestroyWindow( Window );
	glfwTerminate();

	return 0;
}
