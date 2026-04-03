#shader vertex
#version 460 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

layout(location = 1) uniform vec4 u_Color;

void main()
{
	color = u_Color;
}