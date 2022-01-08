#version 330 core
out vec4 FragColor;

in vec3 oNormal;
in vec2 oTexCoord;
in vec3 oFragPos;

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(oNormal);
	vec3 lightDir = normalize(lightPos - oFragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	vec3 result = (ambient + diffuse) * objectColor;
	
	FragColor = vec4(result, 1.0);
}