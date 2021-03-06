#version 430 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexture;
layout(location = 2) in vec3 vNormal;
 


out vec3 myColor;
out vec2 texCoord;

out vec3 Normal;
out vec3 EyeDir;
out vec3 LightDir;

uniform vec3 lightPos;

uniform highp mat4 MVP;
uniform highp mat4 Model;
uniform highp mat4 View;

void main()
{
	
	texCoord = vTexture;
	
	// Vertex position camera space
	vec3 vert_pos = vec3(View * Model * vec4(vPosition, 1));
	
	// Eye direction camera space
	EyeDir = vec3(0, 0, 0) - vert_pos;
	
	// Normal camera space
	Normal = vec3(View * Model * vec4(vNormal, 0));
	
	vec3 LightPos = (View * vec4(lightPos, 1)).xyz;
	// light direction camera camera space
	LightDir = LightPos + EyeDir;

	gl_Position = MVP * vec4(vPosition, 1.f);


}
