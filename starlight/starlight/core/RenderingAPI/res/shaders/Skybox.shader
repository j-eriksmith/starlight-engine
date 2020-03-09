#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoords = position;
	gl_Position = projection * view * vec4(position,1.0f);
}

#shader fragment
#version 330 core

in vec3 TexCoords;
layout(location = 0) out vec4 color;

uniform samplerCube skybox;

void main()
{
	color = vec4(texture(skybox, TexCoords));
	//color = vec4(1.0, 0.0, 0.0, 1.0);
}