#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 u_MVP;

void main()
{
	gl_Position = projection * view * model * position;
}


#shader fragment
#version 330 core

uniform vec4 u_Color;

uniform vec3 lightColor;

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(lightColor, 1.0);
}