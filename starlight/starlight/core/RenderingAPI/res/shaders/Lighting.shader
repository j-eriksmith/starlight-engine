#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 u_MVP;

out vec3 Normal;
out vec4 vertexPosition;

void main()
{
	gl_Position = projection * view * model * vec4(aPosition, 1.0);
	// Multiply our normal vector by the normal matrix. If someone non-uniformly scales an object,
	// the normal matrix will properly adjust the normal vector so that it remains perpendicular to
	// the surface
	Normal = mat3( transpose( inverse( model ) ) ) * aNormal;
	vec4 worldPos = model * vec4(aPosition, 1.0);
	vertexPosition = worldPos;
}


#shader fragment
#version 330 core

uniform vec4 u_Color;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec4 vertexPosition;

layout(location = 0) out vec4 color;

void main()
{
	float ambientStrength = 0.2;
	vec3 ambientColor = lightColor * ambientStrength;
	vec3 norm = normalize(Normal);
	vec3 lightRay = normalize(lightPos - vertexPosition.xyz);
	float diffuseStrength = max(0.0, dot(norm, lightRay));
	vec3 diffuseColor = lightColor * diffuseStrength;

	float specularCoeff = 0.5;
	vec3 viewDir = normalize(viewPos - vertexPosition.xyz);
	vec3 reflectDir = reflect(-lightRay, norm);
	float specularStrength = pow(max(dot(reflectDir, viewDir), 0.0), 128);
	vec3 specularColor = specularCoeff * specularStrength * lightColor;
	color = vec4( (diffuseColor + ambientColor + specularColor) * objectColor, 1.0);
}