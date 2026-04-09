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
	constexpr const char* ColorRed = "\x1b[31m";
	constexpr const char* ColorYellow = "\x1b[33m";
	constexpr const char* ColorMagenta = "\x1b[35m";
	constexpr const char* ColorReset = "\x1b[0m";

	void APIENTRY GLDebugMessageCallback( GLenum InSource, GLenum InType, GLuint Id,
	                                      GLenum InSeverity, GLsizei Length,
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

		int Result;
		glGetProgramiv( ProgramId, GL_LINK_STATUS, &Result );
		if ( Result == GL_FALSE )
		{
			int Length;
			glGetProgramiv( ProgramId, GL_INFO_LOG_LENGTH, &Length );
			auto InfoLog = static_cast<char*>(alloca( Length * sizeof( char ) ));
			glGetProgramInfoLog( ProgramId, Length, &Length, InfoLog );
			std::println( stderr, "Failed to link shader program: {}", InfoLog );
			glDeleteProgram( ProgramId );
			glDeleteShader( VertexShaderId );
			glDeleteShader( FragmentShaderId );
			return 0;
		}

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
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );

	GLFWwindow* Window = glfwCreateWindow( 1280, 720, "DoppioEngine", nullptr, nullptr );
	if ( Window == nullptr )
	{
		std::println( stderr, "Failed to create GLFW window" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( Window );

	glfwSwapInterval( 1 );

	const int Version = gladLoadGLLoader( []( const char* Name ) -> void*
	{
		return reinterpret_cast<void*>(glfwGetProcAddress( Name ));
	} );

	if ( Version == 0 )
	{
		std::println( stderr, "Failed to initialize GLAD" );
		return -1;
	}

	glEnable( GL_DEBUG_OUTPUT );
	glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
	glDebugMessageCallback( GLDebugMessageCallback, nullptr );

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
	ImGui_ImplOpenGL3_Init( "#version 460 core" );

	constexpr float Positions[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f,
	};

	constexpr unsigned int Indices[]
	{
		0, 1, 2,
		2, 3, 0
	};

	unsigned int VertexArray;
	glGenVertexArrays( 1, &VertexArray );
	glBindVertexArray( VertexArray );

	unsigned int VertexBuffer;
	glGenBuffers( 1, &VertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof( float ), Positions, GL_STATIC_DRAW );

	unsigned int IndexBuffer;
	glGenBuffers( 1, &IndexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof( unsigned int ), Indices, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 2, nullptr );

	const ShaderProgramSource ShaderSource = ParseShader( "Engine/Resources/Shaders/Basic.shader" );

	const unsigned int Shader = CreateShader( ShaderSource.VertexSource, ShaderSource.FragmentSource );
	glUseProgram( Shader );

	//const int Location = glGetUniformLocation( Shader, "u_Color" );
	glUniform4f( 1, 0.8f, 0.3f, 0.8f, 1.0f );

	float Red = 0.0f;
	float Increment = 0.01f;

	while ( !glfwWindowShouldClose( Window ) )
	{
		glClear( GL_COLOR_BUFFER_BIT );

		glUniform4f( 1, Red, 0.3f, 0.8f, 1.0f );
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );

		if ( Red > 1.0f )
		{
			Increment = -0.01f;
		}
		else if ( Red < 0.0f )
		{
			Increment = 0.01f;
		}

		Red += Increment;

		glfwSwapBuffers( Window );

		glfwPollEvents();
	}

	glDeleteProgram( Shader );
	glfwDestroyWindow( Window );
	glfwTerminate();

	return 0;
}
