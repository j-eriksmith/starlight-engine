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
<<<<<<< HEAD
	gl_Position = projection * view * model * vec4(position,1.0f);
	TexCoords = texCoord;
=======
<<<<<<< HEAD
	gl_Position = projection * view * model * vec4(position, 1.0f);
	 v_TexCoord = texCoord;
=======
	gl_Position = projection * view * model * vec4(position,1.0f);
	TexCoords = texCoord;
>>>>>>> c35be6687152653f93c14ea94c36a614892395ac
>>>>>>> 56132594b52a0d1939dd24416d20b25e64c1af38
}
	

#shader fragment
#version 330 core

uniform vec4 u_Color; //= vec4(0.0, 1.0, 0.0, 1.0);

in vec2 TexCoords;

uniform vec3 lightColor;

layout(location = 0) out vec4 color;
<<<<<<< HEAD
=======
<<<<<<< HEAD
uniform sampler2D u_Texture;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

void main()
{
	//vec4 texColor = texture(texture_diffuse1, v_TexCoord);
	//color = texColor * vec4(1.0, 1.0, 1.0, 1.0);
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f) * vec4(texture2D(texture_diffuse1, v_TexCoord));
	color = vec4(0.0, 1.0, 0.0, 1.0);
=======
>>>>>>> 56132594b52a0d1939dd24416d20b25e64c1af38
uniform sampler2D texture_diffuse;

void main()
{
	color = vec4(texture2D(texture_diffuse, TexCoords));
<<<<<<< HEAD
=======
>>>>>>> c35be6687152653f93c14ea94c36a614892395ac
>>>>>>> 56132594b52a0d1939dd24416d20b25e64c1af38
}