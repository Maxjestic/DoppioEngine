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
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBufferLayout.h"

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

	Doppio::Render::Shader shader{ "Engine/Resources/Shaders/Basic.shader" };

	vertexArray.Unbind();
	vertexBuffer.Unbind();
	indexBuffer.Unbind();
	shader.Unbind();

	float red = 0.0f;
	float increment = 0.01f;

	while ( !glfwWindowShouldClose( window ) )
	{
		Doppio::Render::Renderer::Clear();
		
		shader.Bind();
		const auto color = glm::vec4( red, 0.3f, 0.8f, 1.0f );
		shader.SetUniform( "u_Color", color );

		Doppio::Render::Renderer::Draw( vertexArray, indexBuffer, shader );

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

	glfwDestroyWindow( window );
	glfwTerminate();

	return 0;
}
