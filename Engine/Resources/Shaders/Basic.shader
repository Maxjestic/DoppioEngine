#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

layout(location = 1) uniform vec4 u_Color;
layout(location = 2) uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
}