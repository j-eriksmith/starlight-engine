#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 u_MVP;

void main()
{
	gl_Position = projection * view * model * vec4(position,1.0f);
	TexCoords = texCoord;
}
	

#shader fragment
#version 330 core

uniform vec4 u_Color; //= vec4(0.0, 1.0, 0.0, 1.0);

in vec2 TexCoords;

uniform vec3 lightColor;

layout(location = 0) out vec4 color;
uniform sampler2D texture_diffuse;

void main()
{
	// color = vec4(texture2D(texture_diffuse, TexCoords));
	color = vec4(0.0, 1.0, 0.0, 1.0);
}