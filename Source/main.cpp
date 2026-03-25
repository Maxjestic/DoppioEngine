#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

static unsigned int CompileShader( const unsigned int ShaderType, const std::string& ShaderCode )
{
	const unsigned int VertexShaderID = glCreateShader( ShaderType );
	const char* SourcePointer = ShaderCode.c_str();
	glShaderSource( VertexShaderID, 1, &SourcePointer, nullptr );
	glCompileShader( VertexShaderID );

	int Result;
	glGetShaderiv( VertexShaderID, GL_COMPILE_STATUS, &Result );
	if ( Result == GL_FALSE )
	{
		int Length;
		glGetShaderiv( VertexShaderID, GL_INFO_LOG_LENGTH, &Length );

		auto InfoLog = static_cast<char*>(alloca( Length*sizeof(char) ));
		glGetShaderInfoLog( VertexShaderID, Length, &Length, InfoLog );

		std::println( stderr, "Failed to compile {} shader: {}",
		              ( ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment" ), InfoLog );

		glDeleteShader( VertexShaderID );
		return 0;
	}

	return VertexShaderID;
}

static unsigned int CreateShader( const std::string& VertexShader, const std::string& FragmentShader )
{
	const unsigned int ProgramID = glCreateProgram();
	const unsigned int VertexShaderID = CompileShader( GL_VERTEX_SHADER, VertexShader );
	const unsigned int FragmentShaderID = CompileShader( GL_FRAGMENT_SHADER, FragmentShader );

	glAttachShader( ProgramID, VertexShaderID );
	glAttachShader( ProgramID, FragmentShaderID );
	glLinkProgram( ProgramID );
	glValidateProgram( ProgramID );

	glDeleteShader( VertexShaderID );
	glDeleteShader( FragmentShaderID );

	return ProgramID;
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

	GLFWwindow* window = glfwCreateWindow( 1280, 720, "DoppioEngine", nullptr, nullptr );
	if ( window == nullptr )
	{
		std::println( stderr, "Failed to create GLFW window" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );

	int version = gladLoadGLLoader( reinterpret_cast<GLADloadproc>(glfwGetProcAddress) );
	if ( version == 0 )
	{
		std::println( stderr, "Failed to initialize GLAD" );
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
	{
		style.WindowRounding = 2.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL( window, true );
	ImGui_ImplOpenGL3_Init( "#version 430 core" );

	float positions[6] = {
		-0.5, -0.5,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	unsigned int VAO;
	glGenVertexArrays( 1, &VAO );
	glBindVertexArray( VAO );

	unsigned int buffer;
	glGenBuffers( 1, &buffer );
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER, 6 * sizeof( float ), positions, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 2, nullptr );

	const std::string VertexShader =
			"#version 430 core\n"
			"\n"
			"layout(location = 0) in vec4 position;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	gl_Position = position;\n"
			"}\n";

	const std::string FragmentShader =
			"#version 430 core\n"
			"\n"
			"layout(location = 0) out vec4 color;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
			"}\n";

	const unsigned int Shader = CreateShader( VertexShader, FragmentShader );

	glUseProgram( Shader );

	while ( !glfwWindowShouldClose( window ) )
	{
		glClear( GL_COLOR_BUFFER_BIT );
		glDrawArrays( GL_TRIANGLES, 0, 3 );

		glfwSwapBuffers( window );

		glfwPollEvents();


		/*
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport();
		ImGui::ShowDemoWindow();
		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable ) {
		    GLFWwindow *backup_current_context = glfwGetCurrentContext();
		    ImGui::UpdatePlatformWindows();
		    ImGui::RenderPlatformWindowsDefault();
		    glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window);
		*/
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow( window );
	glfwTerminate();

	return 0;
}
