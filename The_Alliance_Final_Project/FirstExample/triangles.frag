#version 430 core

in vec2 texCoord;
out vec4 fColor;

in vec3 Normal;
in vec3 EyeDir;
in vec3 LightDir;

uniform sampler2D texture0;



void main()
{
	vec3 lightColor = vec3 (1.f, 1.f, 1.f);
	// Ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightDir);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular
	vec3 viewDir = normalize(EyeDir);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specularStrength = 1.f;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular);
	
	fColor = texture(texture0, texCoord) * vec4(result , 1.0);

	//fColor = texture(texture0, texCoord);

}