#version 330 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec3 vertColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	vec3 distance = lightPos - FragPos;

	// Ambient
	float ambientStrength = 0.14f;
	vec3 ambient = ambientStrength * lightColor / (distance*distance);

	// Diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient +diffuse + specular) * vertColor;//objectColor;
	color = vec4(result, 1.0f);
}